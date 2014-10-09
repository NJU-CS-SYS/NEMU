#include "common.h"
#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */

#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

enum {
	OR, AND,                    // 0, 1 
	BIT_OR, BIT_XOR, BIT_AND,   // 2, 3, 4
	NE, EQ, LE, LS, GE, GT,     // 5, 6, 7, 8, 9, 10
	RSHIFT, LSHIFT,             // 11, 12
	ADD, SUB,                   // 13, 14
	MUL, DIV, MOD,              // 15, 16, 17
	NOT, POINTER, NEG,          // 18, 19, 20
	LBRACKET, RBRACKET,         // 21, 22, 23
	NUM, HEX, REG,              // 24, 25, 26
	NOTYPE                      // 27

	/* TODO: Add more token types */

};
static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TO-DO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// white space
	{"0x[0-9a-f]+", HEX},              // heximal
	{"[0-9]+", NUM},                // decimal
	{"\\$(eax|ecx|edx|ebx|esp|ebp|esi|edi|eip|ax|cx|dx|bx|al|ah|cl|ch|dl|dh|bl|bh)", REG},
	{"\\+", ADD},
	{"-", SUB},
	{"\\*", MUL},
	{"/", DIV},
	{"%", MOD},
	{"<<", LSHIFT},
	{">>", RSHIFT},
	{"<=", LE},
	{">=", GE},
	{"==", EQ},
	{"!=", NE},
	{"<", LS},
	{">", GT},
    {"&&", AND},
	{"\\|\\|", OR},
	{"!", NOT},
	{"&", BIT_AND},
	{"\\|", BIT_OR},
	{"\\^", BIT_XOR},
	{"-", NEG},
	{"\\*", POINTER},
	{"\\(", LBRACKET},
	{"\\)", RBRACKET}
};

/* the size of regex(s) */
#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

/* regex buffer */
static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		/* compiling */
		ret = regcomp(re+i, rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			/* if not success */
			regerror(ret, &re[i], error_msg, 128);
			test(0, "regex compilation failed: %s\n%s\n", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32]; /* buffer */
} Token;

Token tokens[32];
int nr_token; /* the number of token that have been recognized */



static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i++) {
			if(regexec(re + i, e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;
				//Log("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				if (rules[i].token_type != NOTYPE) {
					Token* temp_token = &tokens[nr_token];
					temp_token->type = rules[i].token_type;
					if (substr_len < 32)
						strncpy(temp_token->str, substr_start, substr_len);
					else assert(0);
					/* SINGLE */
					if (temp_token->type == SUB
							|| temp_token->type == MUL) {
						if (nr_token == 0 
								|| !(tokens[nr_token-1].type == NUM 
										|| tokens[nr_token-1].type == HEX
										|| tokens[nr_token-1].type == REG
										|| tokens[nr_token-1].type == RBRACKET))
							switch (temp_token->type) {
								case SUB:
									temp_token->type = NEG;
									break;
								case MUL:
									temp_token->type = POINTER;
							}
					}
					nr_token++;
				}
				/* why ?
				switch(rules[i].token_type) {
					default: assert(0);
				}
				*/
				break;
			}
		}
		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}
	return true; 
}


void substr(int p, int q) {
	int i;
	for (i = p; i <= q; i++)
		printf("%s", tokens[i].str);
	putchar('\n');
}

// check parentheses paired right
// use a parentheses buffer to store paired index of )
static int parent[32];
static bool check_parentheses() {
	/*
	if (tokens[p].type == '(' && tokens[q].type == ')') {
		int buf[32];
		int i, j = 0, rec = -1;
		for (i = p; i <= q; i++) {
			if (tokens[i].type == '(') buf[j++] = i;
			else if (tokens[i].type == ')') {
				if (j == 0) return false;
				rec = buf[--j];
			}
		}
//		printf("p = %d, rec = %d\n", p, rec);
		if (rec == p) return true;
	}
	return false;
	*/
	int count = 0;             // +1 when ( and -1 when ) should be 0 at the end
	int stack[32] = {0};       // to pair parentheses
	int i, j;
	for (i = 0, j = -1; i < 32; i++) {
		if (tokens[i].type == '(') {
			count++;
			stack[++j] = i;
		}
		else if (tokens[i].type == ')') {
			count--;
			parent[i] = stack[j--];
		}
		
		if (count < 0) {
			memset(parent, 33, 32);
			return false;
		}
	}
	if (count > 0) {
		memset(parent, 33, 32);
		return false;
	}
	
	for (i = 0; i < 10; i++) {
		Log("%d", parent[i]);
	}
	assert(count == 0);
	return true;
}

static int find_domn(int p, int q) {
	if (tokens[p].type == NOT
			|| tokens[p].type == NEG
			|| tokens[p].type == POINTER)
		return p;

	int i ;
	int which_token = -1;
	int min_type = NOTYPE;
	for (i = q; i >= p; i--) {
		if (tokens[i].type == RBRACKET) {
			i = parent[i];
			assert(tokens[i].type == LBRACKET);
		}
		else if (tokens[i].type < min_type) {
			min_type = tokens[i].type;
			which_token = i;
		}
	}
	assert(min_type != NOTYPE);
	return which_token;
}

static uint32_t evaluate(int p, int q) {
	if (p > q) assert(0); // bad expression!
	else if (p == q) {
		switch (tokens[p].type) {
			char* rec = NULL;
			char temp[5];
			case NUM:
					  return atoi(tokens[p].str);
			case HEX: 
					  return strtol(tokens[p].str, &rec, 16);
			case REG:
					  strcpy(temp, tokens[p].str);
					  if (!strcmp(temp, "$eax")) return cpu.eax;
					  else if (!strcmp(temp, "$ecx")) return cpu.ecx;
					  else if (!strcmp(temp, "$edx")) return cpu.edx;
					  else if (!strcmp(temp, "$ebx")) return cpu.ebx;
					  else if (!strcmp(temp, "$esp")) return cpu.esp;
					  else if (!strcmp(temp, "$ebp")) return cpu.ebp;
 					  else if (!strcmp(temp, "$esi")) return cpu.esi;
 					  else if (!strcmp(temp, "$edi")) return cpu.edi;
 					  else if (!strcmp(temp, "$eip")) return cpu.eip;
 					  else if (!strcmp(temp, "$ax"))  return reg_w(R_AX);
 					  else if (!strcmp(temp, "$al"))  return reg_b(R_AL);
 					  else if (!strcmp(temp, "$ah"))  return reg_b(R_AH);
 					  else if (!strcmp(temp, "$cx"))  return reg_w(R_CX);
 					  else if (!strcmp(temp, "$cl"))  return reg_b(R_CL);
 					  else if (!strcmp(temp, "$ch"))  return reg_b(R_CH);
 					  else if (!strcmp(temp, "$dx"))  return reg_w(R_DX);
 					  else if (!strcmp(temp, "$dl"))  return reg_b(R_DL);
 					  else if (!strcmp(temp, "$dh"))  return reg_b(R_DH);
 					  else if (!strcmp(temp, "$bx"))  return reg_w(R_BX);
 					  else if (!strcmp(temp, "$bl"))  return reg_b(R_BL);
 					  else if (!strcmp(temp, "$bh"))  return reg_b(R_BH);
		}
	}
	else if (tokens[p].type == LBRACKET && tokens[p].type == RBRACKET) return evaluate(p + 1, q - 1);
	else {
		int op = find_domn(p, q);
		
		int eval2 = evaluate(op + 1, q);

		switch (tokens[op].type) {
			case NOT: return !eval2;
			case NEG: return -eval2;
			case POINTER: return swaddr_read(eval2, 4); //how long?
			default: assert(op != p);
		}

		int eval1 = evaluate(p, op - 1);
		switch(tokens[op].type) {
			case ADD: return eval1 + eval2;
			case SUB: return eval1 - eval2;
			case MUL: return eval1 * eval2;
			case DIV: return eval1 / eval2;
			case MOD: return eval1 % eval2;

			case AND: return eval1 && eval2;
			case OR:  return eval1 || eval2;

			case BIT_OR:  return eval1 | eval2;
			case BIT_XOR: return eval1 ^ eval2;
			case BIT_AND: return eval1 & eval2;

			case EQ: return eval1 == eval2;
			case NE: return eval1 != eval2;
			case LE: return eval1 <= eval2;
			case LS: return eval1 <  eval2;
			case GE: return eval1 >= eval2;
			case GT: return eval1 >  eval2;

			case RSHIFT: return eval1 >> eval2;
			case LSHIFT: return eval1 << eval2;

			default: assert(0);
		}
	}
	return 0;
}
/*
static uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	return evaluate(0, nr_token-1);
}
*/
void test_tokens(char *e) {
	make_token(e);
	substr(0, nr_token-1);
	printf("%d\n", evaluate(0, nr_token-1));
}

uint32_t calculate(char *e) {
	make_token(e);
	if (check_parentheses()) {
		return evaluate(0, nr_token-1);
	}
	printf("Invalid Expression\n");
	return 0;
}
