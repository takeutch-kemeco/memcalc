/* next_memcalc.bison.y
 * Copyright (C) 2012, 2013 Takeutch Kemeco
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
#include "node.h"
#include "node_debug.h"
#include "calcnode.h"

#include "func_putpixel.h"

#define YYMAXDEPTH 0x10000000

void yyerror(const char *s)
{
        printf("%s\n",s);
        exit(1);
}

extern FILE* yyin;
extern FILE* yyout;

%}

%union {
        double realval;
        char identifier_str[0x80];
        struct Node* node;
}

%token __FUNC_BL_PUTC __FUNC_BL_PUTS1 __FUNC_BL_PRINTF __FUNC_BL_SCANF __FUNC_BL_MALLOC __FUNC_BL_RAND __FUNC_BL_SRAND
%token __FUNC_BL_GETS __FUNC_BL_OPENWIN __FUNC_BL_SETCOL __FUNC_BL_SETBCOL __FUNC_BL_RGB __FUNC_BL_ICOL __FUNC_BL_FLSHWIN
%token __FUNC_BL_GETGRPB __FUNC_BL_SETPIX __FUNC_BL_FILLRECT __FUNC_BL_DRAWRECT __FUNC_BL_DRAWLINE __FUNC_BL_RND
%token __FUNC_BL_WAIT __FUNC_BL_COLOR __FUNC_BL_LOCATE __FUNC_BL_GETPIX __FUNC_BL_WAITNF __FUNC_BL_INKEY1 __FUNC_BL_CLS
%token __FUNC_BL_INPTINT __FUNC_BL_INPTFLOT __FUNC_BL_SETMODE __FUNC_BL_FILLOVAL __FUNC_BL_DRAWSTR __FUNC_BL_OPENVWIN
%token __FUNC_BL_SLCTWIN __FUNC_BL_COPYRCT0 __FUNC_BL_COPYRCT1 __FUNC_BL_DRAWPTRN_D __FUNC_BL_DRAWPTRN_R

%token __FUNC_PRINT __FUNC_PUTPIXEL __FUNC_PUTCHAR
%token __STATE_IF __STATE_ELSE
%token __STATE_EXP_IF __STATE_EXP_ELSE
%token __OPE_GOTO __OPE_GOSUB __OPE_RETURN __OPE_LABEL
%token __CONST_FLOAT
%token __OPE_PLUS __OPE_MINUS
%token __OPE_MUL __OPE_DIV __OPE_MOD
%token __OPE_LSHIFT __OPE_RSHIFT
%token __OPE_OR __OPE_AND __OPE_XOR __OPE_NOT __OPE_COMPARISON __OPE_NOT_COMPARISON
%token __OPE_ISSMALL __OPE_ISSMALL_COMP __OPE_ISLARGE __OPE_ISLARGE_COMP
%token __OPE_SUBST

%token __LB __RB
%token __ARRAY_BEGIN __ARRAY_END
%token __BLOCK_BEGIN __BLOCK_END
%token __CAMMA __COLON __BACKSLASH
%token __DECL_END
%token __EOF

%token __EXPRESSION_LIST
%token __IDENTIFIER
%token __DECLARATOR __ASSIGNMENT __READ_VARIABLE __COMPARISON __COMPARISON_UNIT_LIST
%token __SELECTION_IF __SELECTION_EXP
%token __DECLARATION_LIST __DECLARATION_BLOCK
%token __GOTO __GOSUB __RETURN __LABEL
%token __LAMBDA_ABSTRACT __FUNCTION_DESCRIPTION

%left __OPE_SUBST
%left __OPE_COMPARISON __OPE_NOT_COMPARISON __OPE_ISSMALL __OPE_ISSMALL_COMP __OPE_ISLARGE __OPE_ISLARGE_COMP

%left __OPE_LSHIFT __OPE_RSHIFT
%left __OPE_OR __OPE_AND __OPE_XOR __OPE_NOT
%left __OPE_ADD __OPE_SUB
%left __OPE_MUL __OPE_DIV __OPE_MOD
%left __OPE_COMPLEX __OPE_COMPLEX_POLAR
%left __OPE_PLUS __OPE_MINUS
%left __OPE_REAL_PART __OPE_IMAGINARY_PART __OPE_ABSOLUTE __OPE_CONJUGATE __OPE_ARGUMENT __OPE_POWER

%type <realval> __CONST_FLOAT
%type <node> declaration declaration_block declaration_list
%type <node> expression operation
%type <node> identifier
%type <node> function lambda_abstract function_description
%type <node> jump label
%type <node> assignment declarator initializer read_variable
%type <node> comparison comparison_unit selection_if selection_exp

%type <node> func_print func_putpixel func_putchar

%type <node> func_blike
%type <node> func_bl_putc func_bl_puts1 func_bl_printf func_bl_scanf func_bl_malloc func_bl_rand func_bl_srand
%type <node> func_bl_gets func_bl_openWin func_bl_setCol func_bl_setBCol func_bl_rgb func_bl_iCol func_bl_flshWin
%type <node> func_bl_getGrpB func_bl_setPix func_bl_fillRect func_bl_drawRect func_bl_drawLine func_bl_rnd
%type <node> func_bl_wait func_bl_color func_bl_locate func_bl_getPix func_bl_waitNF func_bl_inkey1 func_bl_cls
%type <node> func_bl_inptInt func_bl_inptFlot func_bl_setMode func_bl_fillOval func_bl_drawStr func_bl_openVWin
%type <node> func_bl_slctWin func_bl_copyRct0 func_bl_copyRct1 func_bl_drawPtrn_d func_bl_drawPtrn_r

%type <identifier_str> __IDENTIFIER

%start syntax_tree

%%

syntax_tree
        : declaration_list __EOF {
#ifdef DEBUG_NODE
                node_print_tree($1);
#endif /* DEBUG_NODE */
                calcnode($1);
                YYACCEPT;
        }
        ;

declaration_list
        : declaration {
                struct Node* n0 = node_child($1, 0);
                if (n0->ope == __LABEL) {
                        char* iden_txt = (char*)node_child(n0, 0);
                        printf("<declaration> __LABEL:[%s]\n", iden_txt);

                        jmptbl_add_node(iden_txt, n0);
                }

                $$ = $1;
        }

        | declaration declaration_list {
                struct Node* n0 = node_child($1, 0);
                if (n0->ope == __LABEL) {
                        char* iden_txt = (char*)node_child(n0, 0);
                        printf("<declaration declaration_list> __LABEL:[%s]\n", iden_txt);

                        jmptbl_add_node(iden_txt, $2);
                }

                struct Node* f0 = node_new(__DECLARATION_LIST);
                node_link(f0, $1);
                node_link(f0, $2);

                $$ = f0;
        }
        ;

declaration
        : declaration_block
        | selection_if
        | expression __DECL_END
        | assignment __DECL_END
        | jump __DECL_END
        | label __DECL_END

        | __DECL_END {
                struct Node* tmp = node_new(__DECL_END);
                $$ = tmp;
        }
        ;

declaration_block
        : __BLOCK_BEGIN declaration_list __BLOCK_END {
                struct Node* tmp = node_new(__DECLARATION_BLOCK);
                node_link(tmp, $2);
                $$ = tmp;
        }

        | __BLOCK_BEGIN __BLOCK_END {
                struct Node* tmp = node_new(__DECLARATION_BLOCK);
                $$ = tmp;
        }
        ;

function
        : func_print
        | func_putpixel
        | func_putchar
        | func_blike
        ;

func_print
        : __FUNC_PRINT expression {
                struct Node* tmp = node_new(__FUNC_PRINT);
                node_link(tmp, $2);
                $$ = tmp;
        }
        ;

func_putpixel
        : __FUNC_PUTPIXEL expression expression expression expression expression {
                struct Node* tmp = node_new(__FUNC_PUTPIXEL);
                node_link(tmp, $2);
                node_link(tmp, $3);
                node_link(tmp, $4);
                node_link(tmp, $5);
                node_link(tmp, $6);
                $$ = tmp;
        }
        ;

func_putchar
        : __FUNC_PUTCHAR expression {
                struct Node* tmp = node_new(__FUNC_PUTCHAR);
                node_link(tmp, $2);
                $$ = tmp;
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
        : __FUNC_BL_PUTC expression {
                struct Node* tmp = node_new(__FUNC_BL_PUTC);
                node_link(tmp, $2);
                $$ = tmp;
        }
        ;

func_bl_puts1
        : __FUNC_BL_PUTS1 expression {
                /* undefined */
        }
        ;

func_bl_printf
        : __FUNC_BL_PRINTF expression {
                /* undefined */
        }
        ;

func_bl_scanf
        : __FUNC_BL_SCANF expression {
                /* undefined */
        }
        ;

func_bl_malloc
        : __FUNC_BL_MALLOC expression {
                /* undefined */
        }
        ;

func_bl_rand
        : __FUNC_BL_RAND {
                struct Node* tmp = node_new(__FUNC_BL_RAND);
                $$ = tmp;
        }
        ;

func_bl_srand
        : __FUNC_BL_SRAND expression {
                struct Node* tmp = node_new(__FUNC_BL_SRAND);
                node_link(tmp, $2);
                $$ = tmp;
        }
        ;

func_bl_gets
        : __FUNC_BL_GETS expression {
                /* undefined */
        }
        ;

func_bl_openWin
        : __FUNC_BL_OPENWIN expression expression {
                struct Node* tmp = node_new(__FUNC_BL_OPENWIN);
                node_link(tmp, $2);
                node_link(tmp, $3);
                $$ = tmp;
        }
        ;

func_bl_setCol
        : __FUNC_BL_SETCOL expression {
                struct Node* tmp = node_new(__FUNC_BL_SETCOL);
                node_link(tmp, $2);
                $$ = tmp;
        }
        ;

func_bl_setBCol
        : __FUNC_BL_SETBCOL expression {
                struct Node* tmp = node_new(__FUNC_BL_SETBCOL);
                node_link(tmp, $2);
                $$ = tmp;
        }
        ;

func_bl_rgb
        : __FUNC_BL_RGB expression expression expression {
                struct Node* tmp = node_new(__FUNC_BL_RGB);
                node_link(tmp, $2);
                node_link(tmp, $3);
                node_link(tmp, $4);
                $$ = tmp;
        }
        ;

func_bl_iCol
        : __FUNC_BL_ICOL expression {
                struct Node* tmp = node_new(__FUNC_BL_ICOL);
                node_link(tmp, $2);
                $$ = tmp;
        }
        ;

func_bl_flshWin
        : __FUNC_BL_FLSHWIN expression expression expression expression {
                struct Node* tmp = node_new(__FUNC_BL_FLSHWIN);
                node_link(tmp, $2);
                node_link(tmp, $3);
                node_link(tmp, $4);
                node_link(tmp, $5);
                $$ = tmp;
        }
        ;

func_bl_getGrpB
        : __FUNC_BL_GETGRPB {
                /* undefined */
        }
        ;

func_bl_setPix
        : __FUNC_BL_SETPIX expression expression expression {
                struct Node* tmp = node_new(__FUNC_BL_SETPIX);
                node_link(tmp, $2);
                node_link(tmp, $3);
                node_link(tmp, $4);
                $$ = tmp;
        }
        ;

func_bl_fillRect
        : __FUNC_BL_FILLRECT expression expression expression expression {
                struct Node* tmp = node_new(__FUNC_BL_FILLRECT);
                node_link(tmp, $2);
                node_link(tmp, $3);
                node_link(tmp, $4);
                node_link(tmp, $5);
                $$ = tmp;
        }
        ;

func_bl_drawRect
        : __FUNC_BL_DRAWRECT expression expression expression expression {
                struct Node* tmp = node_new(__FUNC_BL_DRAWRECT);
                node_link(tmp, $2);
                node_link(tmp, $3);
                node_link(tmp, $4);
                node_link(tmp, $5);
                $$ = tmp;
        }
        ;

func_bl_drawLine
        : __FUNC_BL_DRAWLINE expression expression expression expression {
                struct Node* tmp = node_new(__FUNC_BL_DRAWLINE);
                node_link(tmp, $2);
                node_link(tmp, $3);
                node_link(tmp, $4);
                node_link(tmp, $5);
                $$ = tmp;
        }
        ;

func_bl_rnd
        : __FUNC_BL_RND expression {
                struct Node* tmp = node_new(__FUNC_BL_RND);
                node_link(tmp, $2);
                $$ = tmp;
        }
        ;

func_bl_wait
        : __FUNC_BL_WAIT expression {
                struct Node* tmp = node_new(__FUNC_BL_WAIT);
                node_link(tmp, $2);
                $$ = tmp;
        }
        ;

func_bl_color
        : __FUNC_BL_COLOR expression expression {
                struct Node* tmp = node_new(__FUNC_BL_COLOR);
                node_link(tmp, $2);
                node_link(tmp, $3);
                $$ = tmp;
        }
        ;

func_bl_locate
        : __FUNC_BL_LOCATE expression expression {
                struct Node* tmp = node_new(__FUNC_BL_LOCATE);
                node_link(tmp, $2);
                node_link(tmp, $3);
                $$ = tmp;
        }
        ;

func_bl_getPix
        : __FUNC_BL_GETPIX expression expression {
                struct Node* tmp = node_new(__FUNC_BL_GETPIX);
                node_link(tmp, $2);
                node_link(tmp, $3);
                $$ = tmp;
        }
        ;

func_bl_waitNF
        : __FUNC_BL_WAITNF expression {
                struct Node* tmp = node_new(__FUNC_BL_WAITNF);
                node_link(tmp, $2);
                $$ = tmp;
        }
        ;

func_bl_inkey1
        : __FUNC_BL_INKEY1 {
                struct Node* tmp = node_new(__FUNC_BL_INKEY1);
                $$ = tmp;
        }
        ;

func_bl_cls
        : __FUNC_BL_CLS {
                struct Node* tmp = node_new(__FUNC_BL_CLS);
                $$ = tmp;
        }
        ;

func_bl_inptInt
        : __FUNC_BL_INPTINT expression {
                /* undefined */
        }
        ;

func_bl_inptFlot
        : __FUNC_BL_INPTFLOT expression {
                /* undefined */
        }
        ;

func_bl_setMode
        : __FUNC_BL_SETMODE expression {
                struct Node* tmp = node_new(__FUNC_BL_SETMODE);
                node_link(tmp, $2);
                $$ = tmp;
        }
        ;

func_bl_fillOval
        : __FUNC_BL_FILLOVAL expression expression expression expression {
                struct Node* tmp = node_new(__FUNC_BL_FILLOVAL);
                node_link(tmp, $2);
                node_link(tmp, $3);
                node_link(tmp, $4);
                node_link(tmp, $5);
                $$ = tmp;
        }
        ;

func_bl_drawStr
        : __FUNC_BL_DRAWSTR expression {
                /* undefined */
        }
        ;

func_bl_openVWin
        : __FUNC_BL_OPENVWIN expression expression expression {
                struct Node* tmp = node_new(__FUNC_BL_OPENVWIN);
                node_link(tmp, $2);
                node_link(tmp, $3);
                node_link(tmp, $4);
                $$ = tmp;
        }
        ;

func_bl_slctWin
        : __FUNC_BL_SLCTWIN expression {
                struct Node* tmp = node_new(__FUNC_BL_SLCTWIN);
                node_link(tmp, $2);
                $$ = tmp;
        }
        ;

func_bl_copyRct0
        : __FUNC_BL_COPYRCT0 expression expression expression expression
                             expression expression expression expression
        {
                struct Node* tmp = node_new(__FUNC_BL_COPYRCT0);
                node_link(tmp, $2);
                node_link(tmp, $3);
                node_link(tmp, $4);
                node_link(tmp, $5);
                node_link(tmp, $6);
                node_link(tmp, $7);
                node_link(tmp, $8);
                node_link(tmp, $9);
                $$ = tmp;
        }
        ;

func_bl_copyRct1
        : __FUNC_BL_COPYRCT1 expression expression expression expression
                             expression expression expression expression
                             expression
        {
                struct Node* tmp = node_new(__FUNC_BL_COPYRCT1);
                node_link(tmp, $2);
                node_link(tmp, $3);
                node_link(tmp, $4);
                node_link(tmp, $5);
                node_link(tmp, $6);
                node_link(tmp, $7);
                node_link(tmp, $8);
                node_link(tmp, $9);
                node_link(tmp, $10);
                $$ = tmp;
        }
        ;

func_bl_drawPtrn_d
        : __FUNC_BL_DRAWPTRN_D expression expression expression expression
                               expression expression
        {
                /* undefined */
        }
        ;

func_bl_drawPtrn_r
        : __FUNC_BL_DRAWPTRN_R expression expression expression expression
                               expression expression
        {
                /* undefined */
        }
        ;

assignment
        : declarator __OPE_SUBST initializer {
                struct Node* tmp = node_new(__ASSIGNMENT);
                node_link(tmp, $1);
                node_link(tmp, $3);
                $$ = tmp;
        }
        ;

identifier
        : __IDENTIFIER {
                char* iden_text = malloc(sizeof($1) + 1);
                strcpy(iden_text, $1);

                struct Node* tmp = node_new_leaf(__IDENTIFIER, (struct Node*)iden_text);
                $$ = tmp;
        }
        ;

declarator
        : identifier {
                struct Node* tmp = node_new(__DECLARATOR);
                node_link(tmp, $1);
                $$ = tmp;
        }

        | identifier __ARRAY_BEGIN initializer __ARRAY_END {
                struct Node* tmp = node_new(__DECLARATOR);
                node_link(tmp, $1);
                node_link(tmp, $3);
                $$ = tmp;
        }
        ;

initializer
        : assignment
        | expression
        ;

expression
        : operation
        | read_variable
        | comparison
        | selection_exp
        | function
        | lambda_abstract
        | function_description
        ;

operation
        : __CONST_FLOAT {
                struct Complex* tmp = complex_new($1, 0);
                $$ = node_new_leaf(__CONST_FLOAT, (void*)tmp);
        }

        | __OPE_REAL_PART expression {
                struct Node* tmp = node_new(__OPE_REAL_PART);
                node_link(tmp, $2);
                $$ = tmp;
        }

        | __OPE_IMAGINARY_PART expression {
                struct Node* tmp = node_new(__OPE_IMAGINARY_PART);
                node_link(tmp, $2);
                $$ = tmp;
        }

        | __OPE_CONJUGATE expression {
                struct Node* tmp = node_new(__OPE_CONJUGATE);
                node_link(tmp, $2);
                $$ = tmp;
        }

        | __OPE_ARGUMENT expression {
                struct Node* tmp = node_new(__OPE_ARGUMENT);
                node_link(tmp, $2);
                $$ = tmp;
        }

        | __OPE_ABSOLUTE expression __OPE_ABSOLUTE {
                struct Node* tmp = node_new(__OPE_ABSOLUTE);
                node_link(tmp, $2);
                $$ = tmp;
        }

        | expression __OPE_COMPLEX expression {
                struct Node* tmp = node_new(__OPE_COMPLEX);
                node_link(tmp, $1);
                node_link(tmp, $3);
                $$ = tmp;
        }

        | expression __OPE_COMPLEX_POLAR expression {
                struct Node* tmp = node_new(__OPE_COMPLEX_POLAR);
                node_link(tmp, $1);
                node_link(tmp, $3);
                $$ = tmp;
        }

        | expression __OPE_ADD expression {
                struct Node* tmp = node_new(__OPE_ADD);
                node_link(tmp, $1);
                node_link(tmp, $3);
                $$ = tmp;
        }

        | expression __OPE_SUB expression {
                struct Node* tmp = node_new(__OPE_SUB);
                node_link(tmp, $1);
                node_link(tmp, $3);
                $$ = tmp;
        }

        | expression __OPE_MUL expression {
                struct Node* tmp = node_new(__OPE_MUL);
                node_link(tmp, $1);
                node_link(tmp, $3);
                $$ = tmp;
        }

        | expression __OPE_DIV expression {
                struct Node* tmp = node_new(__OPE_DIV);
                node_link(tmp, $1);
                node_link(tmp, $3);
                $$ = tmp;
        }

        | expression __OPE_POWER expression {
                struct Node* tmp = node_new(__OPE_POWER);
                node_link(tmp, $1);
                node_link(tmp, $3);
                $$ = tmp;
        }

        | expression __OPE_MOD expression {
                struct Node* tmp = node_new(__OPE_MOD);
                node_link(tmp, $1);
                node_link(tmp, $3);
                $$ = tmp;
        }

        | expression __OPE_LSHIFT expression {
                struct Node* tmp = node_new(__OPE_LSHIFT);
                node_link(tmp, $1);
                node_link(tmp, $3);
                $$ = tmp;
        }

        | expression __OPE_RSHIFT expression {
                struct Node* tmp = node_new(__OPE_RSHIFT);
                node_link(tmp, $1);
                node_link(tmp, $3);
                $$ = tmp;
        }

        | expression __OPE_OR expression {
                struct Node* tmp = node_new(__OPE_OR);
                node_link(tmp, $1);
                node_link(tmp, $3);
                $$ = tmp;
        }

        | expression __OPE_AND expression {
                struct Node* tmp = node_new(__OPE_AND);
                node_link(tmp, $1);
                node_link(tmp, $3);
                $$ = tmp;
        }

        | expression __OPE_XOR expression {
                struct Node* tmp = node_new(__OPE_XOR);
                node_link(tmp, $1);
                node_link(tmp, $3);
                $$ = tmp;
        }

        | __OPE_NOT expression {
                struct Node* tmp = node_new(__OPE_NOT);
                node_link(tmp, $2);
                $$ = tmp;
        }

        | __OPE_ADD expression %prec __OPE_PLUS {
                struct Node* tmp = node_new(__OPE_PLUS);
                node_link(tmp, $2);
                $$ = tmp;
        }

        | __OPE_SUB expression %prec __OPE_MINUS {
                struct Node* tmp = node_new(__OPE_MINUS);
                node_link(tmp, $2);
                $$ = tmp;
        }

        | __LB expression __RB {
                $$ = $2;
        }
        ;

lambda_abstract
        : __LB __BACKSLASH declarator __COLON initializer __RB {
                struct Node* tmp = node_new(__LAMBDA_ABSTRACT);
                node_link(tmp, $3);
                node_link(tmp, $5);
                $$ = tmp;
        }
        ;

function_description
        : expression expression {
                struct Node* tmp = node_new(__FUNCTION_DESCRIPTION);
                node_link(tmp, $1);
                node_link(tmp, $2);
                $$ = tmp;
        }
        ;

comparison
        : expression comparison_unit {
                struct Node* tmp = node_new(__COMPARISON);
                node_link(tmp, $1);
                node_link(tmp, $2);
                $$ = tmp;
        }
        ;

comparison_unit
        : comparison_unit comparison_unit {
                struct Node* tmp = node_new(__COMPARISON_UNIT_LIST);
                node_link(tmp, $1);
                node_link(tmp, $2);
                $$ = tmp;
        }

        | __OPE_COMPARISON expression {
                struct Node* tmp = node_new(__OPE_COMPARISON);
                node_link(tmp, $2);
                $$ = tmp;
        }

        | __OPE_NOT_COMPARISON expression {
                struct Node* tmp = node_new(__OPE_NOT_COMPARISON);
                node_link(tmp, $2);
                $$ = tmp;
        }

        | __OPE_ISSMALL expression {
                struct Node* tmp = node_new(__OPE_ISSMALL);
                node_link(tmp, $2);
                $$ = tmp;
        }

        | __OPE_ISSMALL_COMP expression {
                struct Node* tmp = node_new(__OPE_ISSMALL_COMP);
                node_link(tmp, $2);
                $$ = tmp;
        }

        | __OPE_ISLARGE expression {
                struct Node* tmp = node_new(__OPE_ISLARGE);
                node_link(tmp, $2);
                $$ = tmp;
        }

        | __OPE_ISLARGE_COMP expression {
                struct Node* tmp = node_new(__OPE_ISLARGE_COMP);
                node_link(tmp, $2);
                $$ = tmp;
        }
        ;

read_variable
        : identifier {
                struct Node* tmp = node_new(__READ_VARIABLE);
                node_link(tmp, $1);
                $$ = tmp;
        }

        | identifier __ARRAY_BEGIN initializer __ARRAY_END {
                struct Node* tmp = node_new(__READ_VARIABLE);
                node_link(tmp, $1);
                node_link(tmp, $3);
                $$ = tmp;
        }
        ;

selection_if
        : __STATE_IF expression declaration_block {
                struct Node* tmp = node_new(__SELECTION_IF);
                node_link(tmp, $2);
                node_link(tmp, $3);
                $$ = tmp;
        }

        | __STATE_IF expression declaration_block __STATE_ELSE declaration_block {
                struct Node* tmp = node_new(__SELECTION_IF);
                node_link(tmp, $2);
                node_link(tmp, $3);
                node_link(tmp, $5);
                $$ = tmp;
        }

selection_exp
        : expression __STATE_EXP_IF expression __STATE_EXP_ELSE expression {
                struct Node* tmp = node_new(__SELECTION_EXP);
                node_link(tmp, $1);
                node_link(tmp, $3);
                node_link(tmp, $5);
                $$ = tmp;
        }
        ;

label
        : __OPE_LABEL identifier {
                struct Node* tmp = node_new(__LABEL);
                node_link(tmp, $2);
                $$ = tmp;
        }
        ;

jump
        : __OPE_GOTO identifier {
                struct Node* tmp = node_new(__GOTO);
                node_link(tmp, $2);
                $$ = tmp;
        }

        | __OPE_GOSUB identifier {
                struct Node* tmp = node_new(__GOSUB);
                node_link(tmp, $2);
                $$ = tmp;
        }

        | __OPE_RETURN {
                struct Node* tmp = node_new(__RETURN);
                $$ = tmp;
        }
        ;

%%
