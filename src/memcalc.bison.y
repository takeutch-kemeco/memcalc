/* memcalc.bison.y
 * Copyright (C) 2012 Takeutch Kemeco
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "mem.h"
#include "jmptbl.h"
#include "func_putpixel.h"

#define YYMAXDEPTH 0x10000000

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
%token __ARRAY_BEGIN __ARRAY_END
%token __BLOCK_BEGIN __BLOCK_END
%token __CAMMA
%token __DECL_END
%token __EOF

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
%start translation_unit

%%

translation_unit
        : declaration
        | translation_unit declaration
        ;

declaration
        : declaration_unit
        | declaration_unit declaration
        | declaration_block
        | declaration_block declaration
        ;

declaration_block
        : __BLOCK_BEGIN declaration __BLOCK_END
        | __BLOCK_BEGIN __BLOCK_END
        ;

declaration_unit
        : __DECL_END

        | selection

        | function

        | expression __DECL_END

        | assignment __DECL_END

        | jump

        | error __DECL_END {
                yyerrok;
        }

        | __EOF {
                YYACCEPT;
        }
        ;

function
        : func_print
        | func_putpixel
        ;

func_print
        : __FUNC_PRINT __LB expression __RB __DECL_END {
                printf("%f\n", $3);
        }
        ;

func_putpixel
        : __FUNC_PUTPIXEL __LB
          expression __CAMMA expression __CAMMA expression __CAMMA
          expression __CAMMA expression
          __RB __DECL_END {
                __func_putpixel($3, $5, $7, $9, $11);
        }
        ;

assignment
        : declarator __OPE_SUBST initializer {
                double* tmp = (double*)($1->address);
                tmp[$1->index] = $3;
        }
        ;

declarator
        : __IDENTIFIER {
                $$ =read_num_var_memtag($1, 0);
        }
        | __IDENTIFIER __ARRAY_BEGIN initializer __ARRAY_END {
                $$ =read_num_var_memtag($1, (size_t)$3);
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
                $$ = read_num_var_value($1, 0);
        }

        | __IDENTIFIER __ARRAY_BEGIN initializer __ARRAY_END {
                $$ = read_num_var_value($1, (size_t)$3);
        }
        ;

if_conditional
        : __STATE_IF expression {
                if ($2 == 0)
                        skip_declaration_block();

                $$ = $2;
        }
        ;

selection
        : if_conditional declaration_block

        | if_conditional declaration_block __STATE_ELSE {
                if ($1 != 0)
                        skip_declaration_block();
        } declaration_block
        ;

jump
        : __STATE_GOTO __IDENTIFIER __DECL_END {
                u_long fpos = jmptbl_seek($2);
                if (fpos == -1) {
                        printf("\n%s\n構文エラー : 存在しないラベルをジャンプ先に指定しました\n\n", $2);
                        exit(1);
                }

                jump_run(fpos);
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
                print_usage();
                exit(1);
        }

        yyin = fopen(argv[1], "rt");
        if (yyin == NULL) {
                print_file_open_err(argv[1]);
                print_usage();
                exit(1);
        }

        yyout = fopen("/dev/null", "w");
        if (yyout == NULL) {
                print_file_open_err("/dev/null");
                print_usage();
                exit(1);
        }

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
