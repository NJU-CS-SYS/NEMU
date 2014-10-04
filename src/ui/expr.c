#include "common.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */

#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, NUM

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// white space
	{"\\+", '+'},					// plus
	{"\\-", '-'},                   // minus
	{"\\*", '*'},                   // multiple
	{"\\/", '/'},                   // divide
	{"\\(", '('},                   // left bracket
	{"\\)", ')'},                   // right bracket
	{"1", NUM},
	{"2", NUM},
	{"3", NUM},
	{"4", NUM},
	{"5", NUM},
	{"6", NUM},
	{"7", NUM},
	{"8", NUM},
	{"9", NUM},
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
				Token* temp_token = &tokens[nr_token];
				temp_token->type = rules[i].token_type;
				printf("%d\n", substr_len);
				nr_token++;
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

void test_tokens(char *e) {
	make_token(e);
	printf("nr_tokens = %d\n", nr_token);
	putchar('\n');
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Implement code to evaluate the expression. */
	assert(0);
	return 0;
}

