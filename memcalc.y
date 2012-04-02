%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "mem.h"
#include "jmptbl.h"
#include "func_putpixel.h"

void yyerror(const char *s)
{
	printf("%s\n",s);
}

extern FILE* yyin;
extern FILE* yyout;

void jump_run(long fpos)
{
	if (fpos != -1) {
		fseek(yyin, fpos, SEEK_SET);
		yyrestart(yyin);
	}
}

#define IF_CONDITIONAL_FLAG_NULL 0x00
#define IF_CONDITIONAL_FLAG_THEN 0x01
#define IF_CONDITIONAL_FLAG_ELSE 0x02

bool skip_flag = false;

%}

%union {
	double val;
	char identifier[0x100];
	struct MemTag* memtag;
	u_int icf;
}

%token __FUNC_PRINT __FUNC_PUTPIXEL
%token __STATE_IF __STATE_THEN __STATE_ELSE
%token __STATE_GOTO
%token __CONST_FLOAT
%token __OPE_PLUS __OPE_MINUS
%token __OPE_MUL __OPE_DIV __OPE_MOD
%token __OPE_LSHIFT __OPE_RSHIFT
%token __OPE_OR __OPE_AND __OPE_XOR __OPE_NOT
%token __OPE_COMPARISON __OPE_NOT_COMPARISON
%token __OPE_ISSMALL __OPE_ISSMALL_COMP __OPE_ISLARGE __OPE_ISLARGE_COMP
%token __OPE_SUBST

%token __LB __RB
%token __BLOCK_BEGIN __BLOCK_END
%token __CAMMA
%token __DECL_END

%token __IDENTIFIER

%left __OPE_SUBST
%left __OPE_COMPARISON __OPE_NOT_COMPARISON
%left __OPE_ISSMALL __OPE_ISSMALL_COMP __OPE_ISLARGE __OPE_ISLARGE_COMP

%left __OPE_LSHIFT __OPE_RSHIFT
%left __OPE_OR __OPE_AND __OPE_XOR __OPE_NOT
%left __OPE_ADD __OPE_SUB
%left __OPE_MUL __OPE_DIV __OPE_MOD
%left __OPE_PLUS __OPE_MINUS

%type <val> __CONST_FLOAT expression initializer read_variable
%type <icf> if_conditional
%type <identifier> __IDENTIFIER
%type <memtag> declarator

%%

translation_unit
	: declaration
	| translation_unit declaration
	;

declaration
	: declaration_unit
	| __BLOCK_BEGIN declaration_unit __BLOCK_END 
	| __BLOCK_BEGIN declaration_unit declaration __BLOCK_END 
	;

declaration_unit
	: __DECL_END

	| selection

	| function

	| expression __DECL_END

	| declarator __OPE_SUBST initializer __DECL_END {
		if (skip_flag == false) {
			double* tmp = (double*)($1->address);
			*tmp = $3;
		}
	}

	| jump

	| error __DECL_END {
		yyerrok;
	}
	;

function
	: func_print
	| func_putpixel
	;

func_print
	: __FUNC_PRINT __LB expression __RB __DECL_END {
		if (skip_flag == false)
			printf("%f\n", $3);
	}
	;

func_putpixel
	: __FUNC_PUTPIXEL __LB
	  expression __CAMMA expression __CAMMA expression __CAMMA
	  expression __CAMMA expression
	  __RB __DECL_END {
		if (skip_flag == false)
			__func_putpixel($3, $5, $7, $9, $11);
	}
	;

declarator
	: __IDENTIFIER {
		if (skip_flag == false) {
			struct MemTag* p = get_ptr_var($1);
			if (p == NULL) {
				push_var($1, sizeof(double));
				p = get_ptr_var($1);

				double* tmp = (double*)(p->address);
				*tmp = 0;
			}

			$$ = p;
		}
	}
	;

initializer
	: expression {
		if (skip_flag == false)
			$$ = $1;
	}
	;

expression
	: __CONST_FLOAT {
		if (skip_flag == false)
			$$ = $1;
	}

	| read_variable {
		if (skip_flag == false)
			$$ = $1;
	}

	| expression __OPE_ADD expression {
		if (skip_flag == false)
			$$ = $1 + $3;
	}

	| expression __OPE_SUB expression {
		if (skip_flag == false)
			$$ = $1 - $3;
	}

	| expression __OPE_MUL expression {
		if (skip_flag == false)
			$$ = $1 * $3;
	}

	| expression __OPE_DIV expression {
		if (skip_flag == false)
			$$ = $1 / $3;
	}

	| expression __OPE_MOD expression {
		if (skip_flag == false)
			$$ = fmod($1, $3);
	}

	| expression __OPE_LSHIFT expression {
		if (skip_flag == false)
			$$ = ((u_long)$1) << ((u_long)$3);
	}

	| expression __OPE_RSHIFT expression {
		if (skip_flag == false)
			$$ = ((u_long)$1) >> ((u_long)$3);
	}

	| expression __OPE_OR expression {
		if (skip_flag == false)
			$$ = ((u_long)$1) | ((u_long)$3);
	}

	| expression __OPE_AND expression {
		if (skip_flag == false)
			$$ = ((u_long)$1) & ((u_long)$3);
	}

	| expression __OPE_XOR expression {
		if (skip_flag == false)
			$$ = ((u_long)$1) ^ ((u_long)$3);
	}

	| __OPE_NOT expression {
		if (skip_flag == false)
			$$ = ~((u_long)$2);
	}

	| __OPE_ADD expression %prec __OPE_PLUS {
		if (skip_flag == false)
			$$ = +$2;
	}

	| __OPE_SUB expression %prec __OPE_MINUS {
		if (skip_flag == false)
			$$ = -$2;
	}

	| __LB expression __RB {
		if (skip_flag == false)
			$$ = $2;
	}

	| expression __OPE_COMPARISON expression {
		if (skip_flag == false) {
			if ($1 == $3)
				$$ = 1;
			else
				$$ = 0;
		}
	}

	| expression __OPE_NOT_COMPARISON expression {
		if (skip_flag == false) {
			if ($1 != $3)
				$$ = 1;
			else
				$$ = 0;
		}
	}

	| expression __OPE_ISSMALL expression {
		if (skip_flag == false) {
			if ($1 < $3)
				$$ = 1;
			else
				$$ = 0;
		}
	}

	| expression __OPE_ISSMALL_COMP expression {
		if (skip_flag == false) {
			if ($1 <= $3)
				$$ = 1;
			else
				$$ = 0;
		}
	}

	| expression __OPE_ISLARGE expression {
		if (skip_flag == false) {
			if ($1 > $3)
				$$ = 1;
			else
				$$ = 0;
		}
	}

	| expression __OPE_ISLARGE_COMP expression {
		if (skip_flag == false) {
			if ($1 >= $3)
				$$ = 1;
			else
				$$ = 0;
		}
	}
	;

read_variable
	: __IDENTIFIER {
		if (skip_flag == false) {
			struct MemTag* p = get_ptr_var($1);
			if (p == NULL) {
				push_var($1, sizeof(double));
				p = get_ptr_var($1);

				double* tmp = (double*)(p->address);
				*tmp = 0;
			}

			double* tmp = (double*)(p->address);
			$$ = *tmp;
		}
	}
	;

if_conditional
	: __STATE_IF expression {
		if (skip_flag == true) {
			$$ = IF_CONDITIONAL_FLAG_NULL;
			skip_flag = true;
		} else {
			if ($2 != 0) {
				$$ = IF_CONDITIONAL_FLAG_THEN;
				skip_flag = false;
			} else {
				$$ = IF_CONDITIONAL_FLAG_ELSE;
				skip_flag = true;
			}
		}
	}
	;
	
selection
	: if_conditional __STATE_THEN declaration {
		skip_flag = false;
	}

	| if_conditional __STATE_THEN declaration __STATE_ELSE {
		switch ($1) {
		case IF_CONDITIONAL_FLAG_NULL:
			skip_flag = true;
			break;

		case IF_CONDITIONAL_FLAG_THEN:
			skip_flag = true;
			break;

		case IF_CONDITIONAL_FLAG_ELSE:
			skip_flag = false;
			break;
		}
	} declaration {
		skip_flag = false;
	}
	; 

jump
	: __STATE_GOTO __IDENTIFIER __DECL_END {
		if (skip_flag == false) {
			u_long fpos = jmptbl_seek($2);
			if (fpos == -1) {
				printf("error: 存在しないラベルを指定しました\n");
				exit(1);
			}

			jump_run(fpos);
		}
	}
	;

%%

static init_func_all(void)
{
	__init_func_putpixel();
}

int main(int argc, char** argv)
{
	if (argc == 1) {
		printf("program [file] \n");
		exit(1);
	}

	yyin = fopen(argv[1], "rt");
	yyout = fopen("/dev/null", "w");


	mem_init();
	jmptbl_init();

	init_func_all();

	start_create_jmptbl();
	while (yylex() != 0) {
	}


	fseek(yyin, 0, SEEK_SET);
	yyrestart(yyin);

	start_main_process();
	yyparse();

	mem_close();
	jmptbl_close();
	return 0;
}
