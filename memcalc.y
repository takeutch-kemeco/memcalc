%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mem.h"
#include "jmptbl.h"

void yyerror(const char *s)
{
	printf("%s\n",s);
}

extern FILE* yyin;

void jump_run(long fpos)
{
	if (fpos != -1) {
		fseek(yyin, fpos, SEEK_SET);
		yyrestart(yyin);
	}
}

%}

%union {
	double val;
	long fpos;
	char identifier[0x100];
	struct MemTag* memtag;
}

%token __FUNC_PRINT
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

%type <val> __CONST_FLOAT expression selection initializer read_variable
%type <fpos> jump
%type <identifier> __IDENTIFIER
%type <memtag> declarator

%%

translation_unit
	: declaration
	| translation_unit declaration
	;

declaration
	: __DECL_END

	| declarator __DECL_END

	| selection

	| func_print

	| expression __DECL_END

	| declarator __OPE_SUBST initializer __DECL_END {
		double* tmp = (double*)($1->address);
		*tmp = $3;
	}

	| jump {
		jump_run($1);
	}

	| error __DECL_END {
		yyerrok;
	}
	;

func_print
	: __FUNC_PRINT expression __DECL_END {
		printf("%f\n", $2);
	}

declarator
	: __IDENTIFIER {
		struct MemTag* p = get_ptr_var($1);
		if (p == NULL) {
			push_var($1, sizeof(double));
			p = get_ptr_var($1);

			double* tmp = (double*)(p->address);
			*tmp = 0;
		}

		$$ = p;
	}
	;

initializer
	: expression {
		$$ = $1;
	}
	;

expression
	: __CONST_FLOAT {
		$$ = $1;
	}

	| read_variable {
		$$ = $1;
	}

	| expression __OPE_ADD expression {
		$$ = $1 + $3;
	}

	| expression __OPE_SUB expression {
		$$ = $1 - $3;
	}

	| expression __OPE_MUL expression {
		$$ = $1 * $3;
	}

	| expression __OPE_DIV expression {
		$$ = $1 / $3;
	}

	| expression __OPE_MOD expression {
		$$ = fmod($1, $3);
	}

	| expression __OPE_LSHIFT expression {
		$$ = ((u_long)$1) << ((u_long)$3);
	}

	| expression __OPE_RSHIFT expression {
		$$ = ((u_long)$1) >> ((u_long)$3);
	}

	| expression __OPE_OR expression {
		$$ = ((u_long)$1) | ((u_long)$3);
	}

	| expression __OPE_AND expression {
		$$ = ((u_long)$1) & ((u_long)$3);
	}

	| expression __OPE_XOR expression {
		$$ = ((u_long)$1) ^ ((u_long)$3);
	}

	| __OPE_NOT expression {
		$$ = ~((u_long)$2);
	}

	| __OPE_ADD expression %prec __OPE_PLUS {
		$$ = +$2;
	}

	| __OPE_SUB expression %prec __OPE_MINUS {
		$$ = -$2;
	}

	| __LB expression __RB {
		$$ = $2;
	}

	| expression __OPE_COMPARISON expression {
		if ($1 == $3)
			$$ = 1;
		else
			$$ = 0;
	}

	| expression __OPE_NOT_COMPARISON expression {
		if ($1 != $3)
			$$ = 1;
		else
			$$ = 0;
	}

	| expression __OPE_ISSMALL expression {
		if ($1 < $3)
			$$ = 1;
		else
			$$ = 0;
	}

	| expression __OPE_ISSMALL_COMP expression {
		if ($1 <= $3)
			$$ = 1;
		else
			$$ = 0;
	}

	| expression __OPE_ISLARGE expression {
		if ($1 > $3)
			$$ = 1;
		else
			$$ = 0;
	}

	| expression __OPE_ISLARGE_COMP expression {
		if ($1 >= $3)
			$$ = 1;
		else
			$$ = 0;
	}
	;

read_variable
	: __IDENTIFIER {
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
	;

selection
	: __STATE_IF expression __STATE_THEN jump {
		if ($2 != 0)
			jump_run($4);
	}

	| __STATE_IF expression __STATE_THEN jump __STATE_ELSE jump {
		if ($2 != 0)
			jump_run($4);
		else
			jump_run($6);
	}
	;

jump
	: __STATE_GOTO __IDENTIFIER __DECL_END {
		$$ = jmptbl_seek($2);
	}

%%

int main(int argc, char** argv)
{
	if (argc == 1) {
		printf("program [file] \n");
		exit(1);
	}

	yyin = fopen(argv[1], "rt");


	mem_init();
	jmptbl_init();


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
