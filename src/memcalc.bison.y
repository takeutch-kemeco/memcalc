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
#include "complex.h"
#include "jmptbl.h"
#include "pc.h"

#include "func_putpixel.h"

#define YYMAXDEPTH 0x10000000

void yyerror(const char *s)
{
        printf("%s\n",s);
}

extern FILE* yyin;
extern FILE* yyout;

extern int yycurline;

extern int yycurbyte;
extern int yynextbyte;

void jump_run(uint32_t fpos)
{
        if (fpos != -1) {
                fseek(yyin, fpos, SEEK_SET);
                yyrestart(yyin);
        }
}

%}

%union {
        fpos_t fpos;
        double realval;
        struct Complex compval;
        char identifier[0x100];
        struct MemTag* memtag;
        uint32_t icf;
        struct Comparison comparisonval;
}

%token __FUNC_BL_PUTC __FUNC_BL_PUTS1 __FUNC_BL_PRINTF __FUNC_BL_SCANF __FUNC_BL_MALLOC __FUNC_BL_RAND __FUNC_BL_SRAND __FUNC_BL_GETS __FUNC_BL_OPENWIN __FUNC_BL_SETCOL __FUNC_BL_SETBCOL __FUNC_BL_RGB __FUNC_BL_ICOL __FUNC_BL_FLSHWIN __FUNC_BL_GETGRPB __FUNC_BL_SETPIX __FUNC_BL_FILLRECT __FUNC_BL_DRAWRECT __FUNC_BL_DRAWLINE __FUNC_BL_RND __FUNC_BL_WAIT __FUNC_BL_COLOR __FUNC_BL_LOCATE __FUNC_BL_GETPIX __FUNC_BL_WAITNF __FUNC_BL_INKEY1 __FUNC_BL_CLS __FUNC_BL_INPTINT __FUNC_BL_INPTFLOT __FUNC_BL_SETMODE __FUNC_BL_FILLOVAL __FUNC_BL_DRAWSTR __FUNC_BL_OPENVWIN __FUNC_BL_SLCTWIN __FUNC_BL_COPYRCT0 __FUNC_BL_COPYRCT1 __FUNC_BL_DRAWPTRN_D __FUNC_BL_DRAWPTRN_R

%token __FUNC_PRINT __FUNC_PUTPIXEL __FUNC_PUTCHAR
%token __STATE_IF __STATE_ELSE
%token __STATE_GOTO __STATE_GOSUB __STATE_RETURN
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
%token __CAMMA __COLON __BACKSLASH
%token __DECL_END
%token __EOF

%token __IDENTIFIER

%left __OPE_SUBST
%left __OPE_COMPARISON __OPE_NOT_COMPARISON __OPE_ISSMALL __OPE_ISSMALL_COMP __OPE_ISLARGE __OPE_ISLARGE_COMP

%left __OPE_LSHIFT __OPE_RSHIFT
%left __OPE_OR __OPE_AND __OPE_XOR __OPE_NOT
%left __OPE_ADD __OPE_SUB
%left __OPE_MUL __OPE_DIV __OPE_MOD
%left __OPE_COMPLEX __OPE_COMPLEX_POLAR
%left __OPE_PLUS __OPE_MINUS
%left __OPE_REAL_PART __OPE_IMAGINARY_PART __OPE_ABSOLUTE __OPE_CONJUGATE __OPE_ARGUMENT __OPE_POWER

%type <fpos> __DECL_END;
%type <realval> __CONST_FLOAT
%type <compval> expression initializer function read_variable assignment comparison
%type <comparisonval> comparison_unit
%type <compval> func_bl_rgb func_bl_iCol func_bl_rnd func_bl_getPix func_bl_inkey1 func_bl_openVWin
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

        | function __DECL_END

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
        : __FUNC_PRINT __LB expression __RB {
                const double re = complex_realpart($3);
                const double im = complex_imagpart($3);

                if (im >= 0.0)
                        printf("%f+%fi\n", re, im);
                else
                        printf("%f%fi\n", re, im);
        }
        ;

func_putpixel
        : __FUNC_PUTPIXEL __LB
          expression __CAMMA expression __CAMMA expression __CAMMA
          expression __CAMMA expression __RB {
                __func_putpixel(complex_realpart($3), complex_realpart($5),
                                complex_realpart($7), complex_realpart($9),
                                complex_realpart($11));
        }
        ;

func_putchar
        : __FUNC_PUTCHAR __LB expression __RB {
                putchar(complex_realpart($3));
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
        : __FUNC_BL_PUTC __LB expression __RB {
                bl_putc((int32_t)complex_realpart($3));
        }
        ;

func_bl_puts1
        : __FUNC_BL_PUTS1 __LB expression __RB {
                /* undefined */
        }
        ;

func_bl_printf
        : __FUNC_BL_PRINTF __LB expression __RB {
                /* undefined */
        }
        ;

func_bl_scanf
        : __FUNC_BL_SCANF __LB expression __RB {
                /* undefined */
        }
        ;

func_bl_malloc
        : __FUNC_BL_MALLOC __LB expression __RB {
                /* undefined */
        }
        ;

func_bl_rand
        : __FUNC_BL_RAND __LB  __RB {
                bl_rand();
        }
        ;

func_bl_srand
        : __FUNC_BL_SRAND __LB expression __RB {
                bl_srand((int32_t)complex_realpart($3));
        }
        ;

func_bl_gets
        : __FUNC_BL_GETS __LB expression __RB {
                /* undefined */
        }
        ;

func_bl_openWin
        : __FUNC_BL_OPENWIN __LB expression __CAMMA expression __RB {
                bl_openWin((int32_t)complex_realpart($3), (int32_t)complex_realpart($5));
        }
        ;

func_bl_setCol
        : __FUNC_BL_SETCOL __LB expression __RB {
                bl_setCol((int32_t)complex_realpart($3));
        }
        ;

func_bl_setBCol
        : __FUNC_BL_SETBCOL __LB expression __RB {
                bl_setBCol((int32_t)complex_realpart($3));
        }
        ;

func_bl_rgb
        : __FUNC_BL_RGB __LB expression __CAMMA expression __CAMMA expression __RB {
                const double re = bl_rgb((uint32_t)complex_realpart($3),
                                         (uint32_t)complex_realpart($5),
                                         (uint32_t)complex_realpart($7));

                $$ = complex_constructor(re, 0);
        }
        ;

func_bl_iCol
        : __FUNC_BL_ICOL __LB expression __RB {
                const double re = bl_iCol((int32_t)complex_realpart($3));

                $$ = complex_constructor(re, 0);
        }
        ;

func_bl_flshWin
        : __FUNC_BL_FLSHWIN __LB expression __CAMMA expression __CAMMA expression __CAMMA expression __RB {
                bl_flshWin((int32_t)complex_realpart($3), (int32_t)complex_realpart($5),
                           (int32_t)complex_realpart($7), (int32_t)complex_realpart($9));
        }
        ;

func_bl_getGrpB
        : __FUNC_BL_GETGRPB __LB __RB {
                /* undefined */
        }
        ;

func_bl_setPix
        : __FUNC_BL_SETPIX __LB expression __CAMMA expression __CAMMA expression __RB {
                bl_setPix((int32_t)complex_realpart($3), (int32_t)complex_realpart($5),
                          (int32_t)complex_realpart($7));
        }
        ;

func_bl_fillRect
        : __FUNC_BL_FILLRECT __LB expression __CAMMA expression __CAMMA expression __CAMMA expression __RB {
                bl_fillRect((int32_t)complex_realpart($3), (int32_t)complex_realpart($5),
                            (int32_t)complex_realpart($7), (int32_t)complex_realpart($9));
        }
        ;

func_bl_drawRect
        : __FUNC_BL_DRAWRECT __LB expression __CAMMA expression __CAMMA expression __CAMMA expression __RB {
                bl_drawRect((int32_t)complex_realpart($3), (int32_t)complex_realpart($5),
                            (int32_t)complex_realpart($7), (int32_t)complex_realpart($9));
        }
        ;

func_bl_drawLine
        : __FUNC_BL_DRAWLINE __LB expression __CAMMA expression __CAMMA expression __CAMMA expression __RB {
                bl_drawLine((int32_t)complex_realpart($3), (int32_t)complex_realpart($5),
                            (int32_t)complex_realpart($7), (int32_t)complex_realpart($9));
        }
        ;

func_bl_rnd
        : __FUNC_BL_RND __LB expression __RB {
                const double re = bl_rnd((int32_t)complex_realpart($3));

                $$ = complex_constructor(re, 0);
        }
        ;

func_bl_wait
        : __FUNC_BL_WAIT __LB expression __RB {
                bl_wait((int32_t)complex_realpart($3));
        }
        ;

func_bl_color
        : __FUNC_BL_COLOR __LB expression __CAMMA expression __RB {
                bl_color((int32_t)complex_realpart($3), (int32_t)complex_realpart($5));
        }
        ;

func_bl_locate
        : __FUNC_BL_LOCATE __LB expression __CAMMA expression __RB {
                bl_locate((int32_t)complex_realpart($3), (int32_t)complex_realpart($5));
        }
        ;

func_bl_getPix
        : __FUNC_BL_GETPIX __LB expression __CAMMA expression __RB {
                const double re = bl_getPix((int32_t)complex_realpart($3),
                                            (int32_t)complex_realpart($5));

                $$ = complex_constructor(re, 0);
        }
        ;

func_bl_waitNF
        : __FUNC_BL_WAITNF __LB expression __RB {
                bl_waitNF((int32_t)complex_realpart($3));
        }
        ;

func_bl_inkey1
        : __FUNC_BL_INKEY1 __LB __RB {
                const double re = bl_inkey1();

                $$ = complex_constructor(re, 0);
        }
        ;

func_bl_cls
        : __FUNC_BL_CLS __LB __RB {
                bl_cls();
        }
        ;

func_bl_inptInt
        : __FUNC_BL_INPTINT __LB expression __RB {
                /* undefined */
        }
        ;

func_bl_inptFlot
        : __FUNC_BL_INPTFLOT __LB expression __RB {
                /* undefined */
        }
        ;

func_bl_setMode
        : __FUNC_BL_SETMODE __LB expression __RB {
                bl_setMode((int32_t)complex_realpart($3));
        }
        ;

func_bl_fillOval
        : __FUNC_BL_FILLOVAL __LB expression __CAMMA expression __CAMMA expression __CAMMA expression __RB {
                bl_fillOval((int32_t)complex_realpart($3), (int32_t)complex_realpart($5),
                            (int32_t)complex_realpart($7), (int32_t)complex_realpart($9));
        }
        ;

func_bl_drawStr
        : __FUNC_BL_DRAWSTR __LB expression __RB {
                /* undefined */
        }
        ;

func_bl_openVWin
        : __FUNC_BL_OPENVWIN __LB expression __CAMMA expression __CAMMA expression __RB {
                bl_openVWin((int32_t)complex_realpart($3), (int32_t)complex_realpart($5),
                            (int32_t)complex_realpart($7));
        }
        ;

func_bl_slctWin
        : __FUNC_BL_SLCTWIN __LB expression __RB {
                bl_slctWin((int32_t)complex_realpart($3));
        }
        ;

func_bl_copyRct0
        : __FUNC_BL_COPYRCT0 __LB
                expression __CAMMA expression __CAMMA expression __CAMMA expression __CAMMA
                expression __CAMMA expression __CAMMA expression __CAMMA expression __RB
        {
                bl_copyRct0((int32_t)complex_realpart($3), (int32_t)complex_realpart($5),
                            (int32_t)complex_realpart($7), (int32_t)complex_realpart($9),
                            (int32_t)complex_realpart($11), (int32_t)complex_realpart($13),
                            (int32_t)complex_realpart($15), (int32_t)complex_realpart($17));
        }
        ;

func_bl_copyRct1
        : __FUNC_BL_COPYRCT1 __LB
                expression __CAMMA expression __CAMMA expression __CAMMA expression __CAMMA
                expression __CAMMA expression __CAMMA expression __CAMMA expression __CAMMA
                expression __RB
        {
                bl_copyRct1((int32_t)complex_realpart($3), (int32_t)complex_realpart($5),
                            (int32_t)complex_realpart($7), (int32_t)complex_realpart($9),
                            (int32_t)complex_realpart($11), (int32_t)complex_realpart($13),
                            (int32_t)complex_realpart($15), (int32_t)complex_realpart($17),
                            (int32_t)complex_realpart($19));
        }
        ;

func_bl_drawPtrn_d
        : __FUNC_BL_DRAWPTRN_D __LB
                expression __CAMMA expression __CAMMA expression __CAMMA expression __CAMMA
                expression __CAMMA expression __RB
        {
                /* undefined */
        }
        ;

func_bl_drawPtrn_r
        : __FUNC_BL_DRAWPTRN_R __LB
                expression __CAMMA expression __CAMMA expression __CAMMA expression __CAMMA
                expression __CAMMA expression __RB
        {
                /* undefined */
        }
        ;

assignment
        : declarator __OPE_SUBST initializer {
                struct Complex* tmp = (struct Complex*)($1->address);
                tmp[$1->index] = $3;
                $$ = $3;
        }
        ;

declarator
        : __IDENTIFIER {
                $$ = read_num_var_memtag($1, 0);
        }
        | __IDENTIFIER __ARRAY_BEGIN initializer __ARRAY_END {
                $$ = read_num_var_memtag($1, (size_t)complex_realpart($3));
        }
        ;

initializer
        : assignment {
                $$ = $1;
        }
        | expression {
                $$ = $1;
        }
        | function {
                $$ = $1;
        }
        ;

expression
        : __CONST_FLOAT {
                $$ = complex_constructor($1, 0);
        }

        | __OPE_REAL_PART expression {
                $$ = complex_realpart_safe($2);
        }

        | __OPE_IMAGINARY_PART expression {
                $$ = complex_imagpart_safe($2);
        }

        | __OPE_CONJUGATE expression {
                $$ = complex_conjugate($2);
        }

        | __OPE_ARGUMENT expression {
                $$ = complex_constructor(complex_imagpart(complex_polar($2)), 0);
        }

        | __OPE_ABSOLUTE expression __OPE_ABSOLUTE {
                $$ = complex_abs($2);
        }

        | expression __OPE_COMPLEX expression {
                $$ = complex_constructor(complex_realpart($1), complex_realpart($3));
        }

        | expression __OPE_COMPLEX_POLAR expression {
                $$ = complex_mk_polar(complex_constructor(complex_realpart($1), complex_realpart($3)));
        }

        | read_variable {
                $$ = $1;
        }

        | expression __OPE_ADD expression {
                $$ = complex_add($1, $3);
        }

        | expression __OPE_SUB expression {
                $$ = complex_sub($1, $3);
        }

        | expression __OPE_MUL expression {
                $$ = complex_mul($1, $3);
        }

        | expression __OPE_DIV expression {
                $$ = complex_div($1, $3);
        }

        | expression __OPE_POWER expression {
                $$ = complex_pow($1, $3);
        }

        | expression __OPE_MOD expression {
                $$ = complex_mod($1, $3);
        }

        | expression __OPE_LSHIFT expression {
                $$ = complex_lshift($1, $3);
        }

        | expression __OPE_RSHIFT expression {
                $$ = complex_rshift($1, $3);
        }

        | expression __OPE_OR expression {
                $$ = complex_or($1, $3);
        }

        | expression __OPE_AND expression {
                $$ = complex_and($1, $3);
        }

        | expression __OPE_XOR expression {
                $$ = complex_xor($1, $3);
        }

        | __OPE_NOT expression {
                $$ = complex_not($2);
        }

        | __OPE_ADD expression %prec __OPE_PLUS {
                $$ = $2;
        }

        | __OPE_SUB expression %prec __OPE_MINUS {
                $$ = complex_constructor(-complex_realpart($2), -complex_imagpart($2));
        }

        | __LB expression __RB {
                $$ = $2;
        }

        | comparison {
                $$ = $1;
        }
        ;

comparison
        : expression comparison_unit {
                $$ = complex_and($2.f($1, $2.expval), $2.retval);
        }
        ;

comparison_unit
        : comparison_unit comparison_unit {
                const struct Complex tmp = $2.f($1.expval, $2.expval);
                $$.retval = complex_and(complex_and(tmp, $2.retval), $1.retval);
                $$.f = $1.f;
                $$.expval = $1.expval;
        }

        | __OPE_COMPARISON expression {
                $$.f = complex_comparison;
                $$.expval = $2;
                $$.retval = complex_constructor(1, 0);
        }

        | __OPE_NOT_COMPARISON expression {
                $$.f = complex_not_comparison;
                $$.expval = $2;
                $$.retval = complex_constructor(1, 0);
        }

        | __OPE_ISSMALL expression {
                $$.f = complex_is_small;
                $$.expval = $2;
                $$.retval = complex_constructor(1, 0);
        }

        | __OPE_ISSMALL_COMP expression {
                $$.f = complex_is_small_comp;
                $$.expval = $2;
                $$.retval = complex_constructor(1, 0);
        }

        | __OPE_ISLARGE expression {
                $$.f = complex_is_large;
                $$.expval = $2;
                $$.retval = complex_constructor(1, 0);
        }

        | __OPE_ISLARGE_COMP expression {
                $$.f = complex_is_large_comp;
                $$.expval = $2;
                $$.retval = complex_constructor(1, 0);
        }
        ;

read_variable
        : __IDENTIFIER {
                $$ = read_num_var_value($1, 0);
        }

        | __IDENTIFIER __ARRAY_BEGIN initializer __ARRAY_END {
                $$ = read_num_var_value($1, (size_t)complex_realpart($3));
        }
        ;

if_conditional
        : __STATE_IF expression {
                const uint32_t re = (uint32_t)complex_realpart($2);

                if (re == 0)
                        skip_declaration_block();

                $$ = re;
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
                uint32_t fpos = jmptbl_seek($2);
                if (fpos == -1) {
                        printf("\n構文エラー : goto で、存在しないラベル %s をジャンプ先に指定しました\n\n", $2);
                        exit(1);
                }

                jump_run(fpos);
                yyclearin;
                yycurbyte = yynextbyte = fpos;
        }

        | __STATE_GOSUB __IDENTIFIER __DECL_END {
                uint32_t fpos = jmptbl_seek($2);
                if (fpos == -1) {
                        printf("\n構文エラー : gosub で、存在しないラベル %s をジャンプ先に指定しました\n\n", $2);
                        exit(1);
                }

                pc_push(yycurbyte);

                jump_run(fpos);
                yyclearin;
                yycurbyte = yynextbyte = fpos;
        }

        | __STATE_RETURN __DECL_END {
                const uint64_t fpos = pc_pop();

                jump_run(fpos);
                yyclearin;
                yycurbyte = yynextbyte = fpos;
        }
        ;

%%
