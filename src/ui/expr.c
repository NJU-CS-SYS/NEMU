#include "common.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */

#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

enum {
	NOTYPE = 256, EQ, NUM

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
	{"\\+", '+'},					// plus
	{"-", '-'},                     // minus
	{"\\*", '*'},                   // multiple
	{"/", '/'},                     // divide
	{"_", '_'},                     // NEG
	{"\\(", '('},                   // left bracket
	{"\\)", ')'},                   // right bracket
	{"[0-9]+", NUM},                // numbers
	{"==", EQ}						// equal
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
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(re + i, e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);

				position += substr_len;


				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */
				if (rules[i].token_type != NOTYPE) {
					Token* temp_token = &tokens[nr_token];
					temp_token->type = rules[i].token_type;
					if (substr_len < 32)
						strncpy(temp_token->str, substr_start, substr_len);
					else assert(0);
					
					/* NEG */
					if (temp_token->type == '-') {
						if (nr_token == 0) { // -123+...
							temp_token->type = '_';
							strcpy(temp_token->str, "_");
						}
						else { // focus on pre character
							int temp_type = tokens[nr_token-1].type;
							if (!(('0' <= temp_type && temp_type <= '9') || temp_type == ')')) {
								temp_token->type = '_';
								strcpy(temp_token->str, "_");
							}
						}
					}
					/* KISS !
					if (temp_token->str[0] == '0') {
						int j;
						for (j = 0; temp_token->str[j] != '\0'; j++)
							if (temp_token->str[j] != '0') break;
						if (temp_token->str[j] == '\0') temp_token->str[1] = '\0';
					}
					*/
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

/**
 * Functions to evaluate the expression
 * check_parentheses(int,int) : check parentheses
 * find_domn(int, int) : get dominator operator
 * expr(char*, bool*) : evaluate expressions
 */
bool check_parentheses(int p, int q) {
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
}

int find_domn(int p, int q) {
//	printf("In the find_domn: ");
//	substr(p, q);
	int i, j;
	bool inParentheses = false;
	for (i = 0; i < NR_REGEX; i++) {
//		printf("current compare op = %c\n", rules[i].token_type);
		for (j = q; j >= p; j--) {
//			Log("inParentheses %d", inParentheses);
			if (tokens[j].type == ')') inParentheses = true;
			else if (tokens[j].type == '(' && !inParentheses) assert(0);
			else if (tokens[j].type == '(' && inParentheses) {
				inParentheses = false;
				continue;
			}
			if (inParentheses) continue;
			if (tokens[j].type == rules[i].token_type) {
//				Log("The dominator is %c", tokens[j].type);
				return j;
			}
		}
	}
	Log("Unreachable area!");
	assert(0);
	return -1;
}

int evaluate(int p, int q) {
//	Log("p = %d, q = %d", p, q);
	if (p > q) assert(0); // bad expression!
	else if (p == q) return atoi(tokens[p].str);
	else if (check_parentheses(p, q)) return evaluate(p + 1, q - 1);
	else {
		int op = find_domn(p, q);
		if (tokens[op].type == '_') {
			assert(op == p);
			assert(op+1 <= q);
			return -evaluate(op + 1, q);
		}
		int eval1 = evaluate(p, op - 1);
		int eval2 = evaluate(op + 1, q);
		switch(tokens[op].type) {
			case '+': return eval1 + eval2;
			case '-': return eval1 - eval2;
			case '*': return eval1 * eval2;
			case '/': return eval1 / eval2;
			default: assert(0);
		}
	}
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Implement code to evaluate the expression. */
	return evaluate(0, nr_token-1);
}


void test_tokens(char *e) {
	make_token(e);
	substr(0, nr_token-1);
	printf("%d\n", evaluate(0, nr_token-1));
}
