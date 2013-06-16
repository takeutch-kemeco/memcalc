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

#include <blike0.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
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

%token __FUNC_BL_PUTC __FUNC_BL_PUTS1 __FUNC_BL_PRINTF __FUNC_BL_SCANF __FUNC_BL_MALLOC __FUNC_BL_RAND __FUNC_BL_SRAND __FUNC_BL_GETS __FUNC_BL_OPENWIN __FUNC_BL_SETCOL __FUNC_BL_SETBCOL __FUNC_BL_RGB __FUNC_BL_ICOL __FUNC_BL_FLSHWIN __FUNC_BL_GETGRPB __FUNC_BL_SETPIX __FUNC_BL_FILLRECT __FUNC_BL_DRAWRECT __FUNC_BL_DRAWLINE __FUNC_BL_RND __FUNC_BL_WAIT __FUNC_BL_COLOR __FUNC_BL_LOCATE __FUNC_BL_GETPIX __FUNC_BL_WAITNF __FUNC_BL_INKEY1 __FUNC_BL_CLS __FUNC_BL_INPTINT __FUNC_BL_INPTFLOT __FUNC_BL_SETMODE __FUNC_BL_FILLOVAL __FUNC_BL_DRAWSTR __FUNC_BL_OPENVWIN __FUNC_BL_SLCTWIN __FUNC_BL_COPYRCT0 __FUNC_BL_COPYRCT1 __FUNC_BL_DRAWPTRN_D __FUNC_BL_DRAWPTRN_R

%token __FUNC_PRINT __FUNC_PUTPIXEL __FUNC_PUTCHAR
%token __STATE_IF __STATE_ELSE
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
%type <val> func_bl_rgb func_bl_iCol func_bl_rnd func_bl_getPix func_bl_inkey1 func_bl_openVWin
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
        | func_putchar
	| func_blike
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

func_putchar
        : __FUNC_PUTCHAR __LB expression __RB __DECL_END {
                putchar($3);
        }
        ;

func_blike
	: func_bl_putc
	| func_bl_puts1
	| func_bl_printf
	| func_bl_scanf
	| func_bl_malloc
	| func_bl_rand
	| func_bl_srand
	| func_bl_gets
	| func_bl_openWin
	| func_bl_setCol
	| func_bl_setBCol
	| func_bl_rgb
	| func_bl_iCol
	| func_bl_flshWin
	| func_bl_getGrpB
	| func_bl_setPix
	| func_bl_fillRect
	| func_bl_drawRect
	| func_bl_drawLine
	| func_bl_rnd
	| func_bl_wait
	| func_bl_color
	| func_bl_locate
	| func_bl_getPix
	| func_bl_waitNF
	| func_bl_inkey1
	| func_bl_cls
	| func_bl_inptInt
	| func_bl_inptFlot
	| func_bl_setMode
	| func_bl_fillOval
	| func_bl_drawStr
	| func_bl_openVWin
	| func_bl_slctWin
	| func_bl_copyRct0
	| func_bl_copyRct1
	| func_bl_drawPtrn_d
	| func_bl_drawPtrn_r
	;

func_bl_putc
	: __FUNC_BL_PUTC __LB expression __RB __DECL_END {
		bl_putc($3);
	}
	;

func_bl_puts1
	: __FUNC_BL_PUTS1 __LB expression __RB __DECL_END {
		/* undefined */
	}
	;

func_bl_printf
	: __FUNC_BL_PRINTF __LB expression __RB __DECL_END {
		/* undefined */
	}
	;

func_bl_scanf
	: __FUNC_BL_SCANF __LB expression __RB __DECL_END {
		/* undefined */
	}
	;

func_bl_malloc
      	: __FUNC_BL_MALLOC __LB expression __RB __DECL_END {
		/* undefined */
	}
	;

func_bl_rand
       	: __FUNC_BL_RAND __LB  __RB __DECL_END {
		bl_rand();
	}
	;

func_bl_srand
	: __FUNC_BL_SRAND __LB expression __RB __DECL_END {
		bl_srand($3);
	}
	;

func_bl_gets
	: __FUNC_BL_GETS __LB expression __RB __DECL_END {
		/* undefined */
	}
	;

func_bl_openWin
	: __FUNC_BL_OPENWIN __LB expression __CAMMA expression __RB __DECL_END {
		bl_openWin($3, $5);
	}
	;

func_bl_setCol
	: __FUNC_BL_SETCOL __LB expression __RB __DECL_END {
		bl_setCol($3);
	}
	;

func_bl_setBCol
	: __FUNC_BL_SETBCOL __LB expression __RB __DECL_END {
		bl_setBCol($3);
	}
	;

func_bl_rgb
	: __FUNC_BL_RGB __LB expression __CAMMA expression __CAMMA expression __RB __DECL_END {
		$$ = (uint32_t)bl_rgb($3, $5, $7);
	}
	;

func_bl_iCol
	: __FUNC_BL_ICOL __LB expression __RB __DECL_END {
		$$ = bl_iCol($3);
	}
	;

func_bl_flshWin
	: __FUNC_BL_FLSHWIN __LB expression __CAMMA expression __CAMMA expression __CAMMA expression __RB __DECL_END {
		bl_flshWin($3, $5, $7, $9);
	}
	;

func_bl_getGrpB
	: __FUNC_BL_GETGRPB __LB __RB __DECL_END {
		/* undefined */
	}
	;

func_bl_setPix
	: __FUNC_BL_SETPIX __LB expression __CAMMA expression __CAMMA expression __RB __DECL_END {
		bl_setPix($3, $5, $7);
	}
	;

func_bl_fillRect
	: __FUNC_BL_FILLRECT __LB expression __CAMMA expression __CAMMA expression __CAMMA expression __RB __DECL_END {
		bl_fillRect($3, $5, $7, $9);
	}
	;

func_bl_drawRect
	: __FUNC_BL_DRAWRECT __LB expression __CAMMA expression __CAMMA expression __CAMMA expression __RB __DECL_END {
		bl_drawRect($3, $5, $7, $9);
	}
	;

func_bl_drawLine
	: __FUNC_BL_DRAWLINE __LB expression __CAMMA expression __CAMMA expression __CAMMA expression __RB __DECL_END {
		bl_drawLine($3, $5, $7, $9);
	}
	;

func_bl_rnd
	: __FUNC_BL_RND __LB expression __RB __DECL_END {
		$$ = bl_rnd($3);
	}
	;

func_bl_wait
	: __FUNC_BL_WAIT __LB expression __RB __DECL_END {
		bl_wait($3);
	}
	;

func_bl_color
	: __FUNC_BL_COLOR __LB expression __CAMMA expression __RB __DECL_END {
		bl_color($3, $5);
	}
	;

func_bl_locate
	: __FUNC_BL_LOCATE __LB expression __CAMMA expression __RB __DECL_END {
		bl_locate($3, $5);
	}
	;

func_bl_getPix
	: __FUNC_BL_GETPIX __LB expression __CAMMA expression __RB __DECL_END {
		$$ = bl_getPix($3, $5);
	}
	;

func_bl_waitNF
	: __FUNC_BL_WAITNF __LB expression __RB __DECL_END {
		bl_waitNF($3);
	}
	;

func_bl_inkey1
	: __FUNC_BL_INKEY1 __LB expression __RB __DECL_END {
		$$ = bl_inkey1($3);
	}
	;

func_bl_cls
	: __FUNC_BL_CLS __LB expression __RB __DECL_END {
		bl_cls();
	}
	;

func_bl_inptInt
	: __FUNC_BL_INPTINT __LB expression __RB __DECL_END {
		/* undefined */
	}
	;

func_bl_inptFlot
	: __FUNC_BL_INPTFLOT __LB expression __RB __DECL_END {
		/* undefined */
	}
	;

func_bl_setMode
	: __FUNC_BL_SETMODE __LB expression __RB __DECL_END {
		bl_setMode($3);
	}
	;

func_bl_fillOval
	: __FUNC_BL_FILLOVAL __LB expression __CAMMA expression __CAMMA expression __CAMMA expression __RB __DECL_END {
		bl_fillOval($3, $5, $7, $9);
	}
	;

func_bl_drawStr
	: __FUNC_BL_DRAWSTR __LB expression __RB __DECL_END {
		/* undefined */
	}
	;

func_bl_openVWin
	: __FUNC_BL_OPENVWIN __LB expression __CAMMA expression __CAMMA expression __RB __DECL_END
	{
		bl_openVWin($3, $5, $7);
	}
	;

func_bl_slctWin
	: __FUNC_BL_SLCTWIN __LB expression __RB __DECL_END {
		bl_slctWin($3);
	}
	;

func_bl_copyRct0
	: __FUNC_BL_COPYRCT0 __LB 
		expression __CAMMA expression __CAMMA expression __CAMMA expression __CAMMA
		expression __CAMMA expression __CAMMA expression __CAMMA expression __RB __DECL_END
	{
		bl_copyRct0($3, $5, $7, $9, $11, $13, $15, $17);
	}
	;

func_bl_copyRct1
	: __FUNC_BL_COPYRCT1 __LB 
		expression __CAMMA expression __CAMMA expression __CAMMA expression __CAMMA
		expression __CAMMA expression __CAMMA expression __CAMMA expression __CAMMA
		expression __RB __DECL_END
	{
		bl_copyRct1($3, $5, $7, $9, $11, $13, $15, $17, $19);
	}
	;

func_bl_drawPtrn_d
	: __FUNC_BL_DRAWPTRN_D __LB
		expression __CAMMA expression __CAMMA expression __CAMMA expression __CAMMA
		expression __CAMMA expression __RB __DECL_END
	{
		/* undefined */
	}
	;

func_bl_drawPtrn_r
	: __FUNC_BL_DRAWPTRN_R __LB
		expression __CAMMA expression __CAMMA expression __CAMMA expression __CAMMA
		expression __CAMMA expression __RB __DECL_END
	{
		/* undefined */
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

void init_func_all(void)
{
}
