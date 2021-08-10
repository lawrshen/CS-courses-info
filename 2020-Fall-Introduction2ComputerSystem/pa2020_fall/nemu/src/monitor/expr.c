#include "nemu.h"
#include "cpu/reg.h"
#include "cpu/cpu.h"
#include "memory/memory.h"

#include <stdlib.h>

uint32_t look_up_symtab(char *sym, bool *success);

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum
{
	NOTYPE = 256,
	EQ,
	NEQ,
	NUM,
	HEX,
	REG,
	SYMB,
	LS,
	RS,
	NG,
	NL,
	AND,
	OR,
	DEREF, 
	NEG

	/* TODO: Add more token types */

};

static struct rule
{
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +", NOTYPE}, // white space
	{"==",EQ},
	{"!=",NEQ},
	{"\\+", '+'},
	{"-", '-'},
	{"\\*", '*'},
	{"/",'/'},
	{"%",'%'},
	{"\\(", '('},
	{"\\)", ')'},
	{"[0-9]+",NUM},
	{"0[xX][0-9a-fA-F]+",HEX},
	{"[a-zA-Z_]+",SYMB},
	{"\\$e[abcd]x|\\$e[bi]p|\\$e[sd]i",REG},
	{"<<", LS},						//left shift
	{">>", RS},						//right shift
	{"<=", NG},						//not greater than
	{">=", NL},						//not less than
	{"<", '<'},
	{">", '>'},
	{"&&",AND},
	{"\\|\\|",OR},
	{"&",'&'},
	{"\\^",'^'},
	{"\\|",'|'},
	{"!",'!'},
	{"~",'~'}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
	int i;
	char error_msg[128];
	int ret;

	for (i = 0; i < NR_REGEX; i++)
	{
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0)
		{
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token
{
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e)
{
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;

	while (e[position] != '\0')
	{
		/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i++)
		{
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
			{
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

			//	printf("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */
                int j=0;
				switch (rules[i].token_type)
				{
				case NOTYPE:break;
				case NUM:
				case HEX:
				case REG:
				case SYMB:
				for(j=0;j<substr_len;j++)
				    tokens[nr_token].str[j]=*(substr_start+j);
				tokens[nr_token].str[j]='\0';
				default:
					tokens[nr_token].type = rules[i].token_type;
					nr_token++;
				}

				break;
			}
		}

		if (i == NR_REGEX)
		{
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}
static struct op_prior {
	int operand;
	int priority;
} op_priors[] = {
        {OR,10},
        {AND,9},
        {'|',8},
        {'^',7},
        {'&',6},
        {EQ,5},
        {NEQ,5},
        {'>',4},
        {'<',4},
        {NG,4},
        {NL,4},
        {LS,3},
        {RS,3},
        {'+',2},
        {'-',2},
        {'*',1},
        {'/',1},
        {'%',1},
        {'!',0},
        {'~',0},
        {DEREF,0},
        {NEG,0},
    };
#define NR_OP_Prior (sizeof(op_priors) / sizeof(op_priors[0]))

bool isoperand(int i){
	return tokens[i].type!=NOTYPE && tokens[i].type!=NUM && tokens[i].type!=REG && tokens[i].type!=SYMB && tokens[i].type!=HEX && tokens[i].type!='(' && tokens[i].type!=')';
}
bool check_parentheses(int p,int q){
    int i=p,num=0;
    for(;i<q;i++){
        if(tokens[i].type=='(')num++;
        else if(tokens[i].type==')')num--;
        if(num==0)return false;
    }
    if(tokens[q].type==')')num--;
    return num==0 && tokens[p].type=='(' && tokens[q].type==')' ;
}

uint32_t eval(int p, int q, bool *success)
{
    if(p > q) {
        /* Bad expression */
        *success=false;
        return 0;
    }
    else if(p == q) { 
        /* Single token.
         * For now this token should be a number. 
         * Return the value of the number.
         */ 
        if(tokens[p].type==HEX)
		{
			uint32_t result=0;
			sscanf(tokens[p].str,"%x",&result);
			return result;
		}
		else if(tokens[p].type==NUM)
		{
			uint32_t result=0;
			sscanf(tokens[p].str,"%d",&result);
			return result;
		}
		else if(tokens[p].type==REG)
		{
			if(tokens[p].str[2]=='a')
				return cpu.eax;
			else if(tokens[p].str[2]=='b')
				return cpu.ebx;
			else if(tokens[p].str[2]=='c')
				return cpu.ecx;
			else if(tokens[p].str[2]=='d')
				return cpu.edx;
			else if(tokens[p].str[2]=='s'&&tokens[p].str[3]=='i')
				return cpu.esi;
			else if(tokens[p].str[2]=='d'&&tokens[p].str[3]=='i')
				return cpu.edi;
			else if(tokens[p].str[2]=='s'&&tokens[p].str[3]=='p')
				return cpu.esp;
			else if(tokens[p].str[2]=='b'&&tokens[p].str[3]=='p')
				return cpu.ebp;
			else
				return cpu.eip;
		}
		else
			return look_up_symtab(tokens[p].str, success);
    }
    else if(check_parentheses(p, q) == true) {
        /* The expression is surrounded by a matched pair of parentheses. 
         * If that is the case, just throw away the parentheses.
         */
        return eval(p + 1, q - 1,success); 
    }
    else {
        /* We should do more things here. */
        int op=-1,op_priority=0;
		for(int i=p;i<=q;i++)
		{
			if(tokens[i].type=='(')
			{
				int k=1;
				while(k!=0)
				{
					i++;
					if(tokens[i].type=='(')
						k++;
					else if(tokens[i].type==')')
						k--;
				}
			}
			else if(isoperand(i))
			{
				int j;
				for(j=0;j<NR_OP_Prior;j++)
					if(op_priors[j].operand==tokens[i].type)
						break;
				if(op_priors[j].priority>=op_priority)
				{
					op_priority=op_priors[j].priority;
					op=i;
				}
			}
		}
		int op_type=tokens[op].type;
		uint32_t val1=0,val2=0;
		if(op_type!='!'&&op_type!='~'&&op_type!=NEG&&op_type!=DEREF)
			val1 = eval(p, op - 1, success);
		val2 = eval(op + 1, q, success);
		switch(op_type) {
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': return val1 / val2;
			case '%': return val1 % val2;
			case LS: return val1 << val2;
			case RS: return val1 >> val2;
			case '>': return val1 > val2;
			case '<': return val1 < val2;
			case NG: return val1 <= val2;
			case NL: return val1 >= val2;
			case EQ: return val1 == val2;
			case NEQ: return val1 != val2;
			case '&': return val1 & val2;
			case '^': return val1 ^ val2;
			case '|': return val1 | val2;
			case AND: return val1 && val2;
			case OR: return val1 || val2;
			case '!': return !val2;
			case '~': return ~val2;
			case DEREF: return *(hw_mem+val2);
			case NEG: return -val2;
			default: assert(0);
		}
    }
}
uint32_t expr(char *e, bool *success)
{
	if (!make_token(e))
	{
		*success = false;
		return 0;
	}
    
// 	printf("\nPlease implement expr at expr.c\n");
// 	fflush(stdout);
// 	assert(0);
    for(int i = 0; i < nr_token; i++) {
	if(tokens[i].type == '*' && (i == 0 || isoperand(i-1)))
		tokens[i].type = DEREF;
	else if(tokens[i].type == '-' && (i == 0 || isoperand(i-1)))
		tokens[i].type = NEG;
	}
	return eval(0,nr_token-1,success);
}
