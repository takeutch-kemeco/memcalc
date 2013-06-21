/* memcalc.flex.l
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
#include <stdint.h>
#include "mem.h"
#include "jmptbl.h"
#include "memcalc.bison.h"

long create_jmptbl_ccount;

int skip_declaration_block_depth;

%}

%x create_jmptbl
%x main_process
%x main_process_comment
%x main_process_char
%x main_process_skip_declaration_block

%%

<create_jmptbl>[:][_a-zA-Z][_0-9a-zA-Z]* {
        char* tmp = yytext + 1;

        create_jmptbl_ccount += yyleng;
        jmptbl_add(tmp, create_jmptbl_ccount);
}

<create_jmptbl>.|[\n\r] {
        create_jmptbl_ccount += yyleng;
}

<main_process>[/][*] {
        /* c スタイルのコメントの開始点 */
        BEGIN(main_process_comment);
}

<main_process_comment>[*][/] {
        /* c スタイルのコメントの終了点 */
        BEGIN(main_process);
}

<main_process_comment>. {
        /* c スタイルのコメントの本文 */
}

<main_process>"//".*[\n] {
        /* c++ スタイルのコメント */
}

<main_process>['] {
        /* 一文字リテラルの開始点 */
        BEGIN(main_process_char);
}

<main_process_char>['] {
        /* 一文字リテラルの終了点 */
        BEGIN(main_process);
}

<main_process_char>[\\][n] {
        yylval.realval = '\n';
        return(__CONST_FLOAT);
}

<main_process_char>[\\][0] {
        yylval.realval = '\0';
        return(__CONST_FLOAT);
}

<main_process_char>[\\][\\] {
        yylval.realval = '\\';
        return(__CONST_FLOAT);
}

<main_process_char>[\\]['] {
        yylval.realval = '\'';
        return(__CONST_FLOAT);
}

<main_process_char>[\\]["] {
        yylval.realval = '\"';
        return(__CONST_FLOAT);
}

<main_process_char>. {
        yylval.realval = yytext[0];
        return(__CONST_FLOAT);
}

<main_process>[0][0-9]+[.]?[0-9]* {
        printf("\n%s\n構文エラー : 数値の先頭に不適切な 0 があります\n\n", yytext);
        exit(1);
}

<main_process>[0-9]+[.]?[0-9]* {
        yylval.realval = atof(yytext);
        return(__CONST_FLOAT);
}

<main_process>[0][.][0-9]* {
        yylval.realval = atof(yytext);
        return(__CONST_FLOAT);
}

<main_process>":+"      return(__OPE_COMPLEX);
<main_process>":<"      return(__OPE_COMPLEX_POLAR);
<main_process>"Re"      return(__OPE_REAL_PART);
<main_process>"Im"      return(__OPE_IMAGINARY_PART);
<main_process>"~"       return(__OPE_CONJUGATE);
<main_process>"|"       return(__OPE_ABSOLUTE);
<main_process>"arg"     return(__OPE_ARGUMENT);

<main_process>"+"       return(__OPE_ADD);
<main_process>"-"       return(__OPE_SUB);
<main_process>"*"       return(__OPE_MUL);
<main_process>"/"       return(__OPE_DIV);
<main_process>"%"       return(__OPE_MOD);

<main_process>"<<"      return(__OPE_LSHIFT);
<main_process>">>"      return(__OPE_RSHIFT);

<main_process>"or"      return(__OPE_OR);
<main_process>"and"     return(__OPE_AND);
<main_process>"xor"     return(__OPE_XOR);
<main_process>"not"     return(__OPE_NOT);

<main_process>"=="      return(__OPE_COMPARISON);
<main_process>"/="      return(__OPE_NOT_COMPARISON);
<main_process>"<"       return(__OPE_ISSMALL);
<main_process>"<="      return(__OPE_ISSMALL_COMP);
<main_process>">"       return(__OPE_ISLARGE);
<main_process>">="      return(__OPE_ISLARGE_COMP);

<main_process>"="       return(__OPE_SUBST);

<main_process>"("       return(__LB);
<main_process>")"       return(__RB);

<main_process>"{"       return(__BLOCK_BEGIN);
<main_process>"}"       return(__BLOCK_END);

<main_process>","       return(__CAMMA);
<main_process>";"       return(__DECL_END);

<main_process>"if"      return(__STATE_IF);
<main_process>"else"    return(__STATE_ELSE);

<main_process>"["       return(__ARRAY_BEGIN);
<main_process>"]"       return(__ARRAY_END);

<main_process>"goto"    return(__STATE_GOTO);

<main_process>"print"           return(__FUNC_PRINT);
<main_process>"putpixel"        return(__FUNC_PUTPIXEL);
<main_process>"putchar"         return(__FUNC_PUTCHAR);

<main_process>"bl_putc"         return(__FUNC_BL_PUTC);
<main_process>"bl_puts1"        return(__FUNC_BL_PUTS1);
<main_process>"bl_printf"       return(__FUNC_BL_PRINTF);
<main_process>"bl_scanf"        return(__FUNC_BL_SCANF);
<main_process>"bl_malloc"       return(__FUNC_BL_MALLOC);
<main_process>"bl_rand"         return(__FUNC_BL_RAND);
<main_process>"bl_srand"        return(__FUNC_BL_SRAND);
<main_process>"bl_gets"         return(__FUNC_BL_GETS);
<main_process>"bl_openWin"      return(__FUNC_BL_OPENWIN);
<main_process>"bl_setCol"       return(__FUNC_BL_SETCOL);
<main_process>"bl_setBCol"      return(__FUNC_BL_SETBCOL);
<main_process>"bl_rgb"          return(__FUNC_BL_RGB);
<main_process>"bl_iCol"         return(__FUNC_BL_ICOL);
<main_process>"bl_flshWin"      return(__FUNC_BL_FLSHWIN);
<main_process>"bl_getGrpB"      return(__FUNC_BL_GETGRPB);
<main_process>"bl_setPix"       return(__FUNC_BL_SETPIX);
<main_process>"bl_fillRect"     return(__FUNC_BL_FILLRECT);
<main_process>"bl_drawRect"     return(__FUNC_BL_DRAWRECT);
<main_process>"bl_drawLine"     return(__FUNC_BL_DRAWLINE);
<main_process>"bl_rnd"          return(__FUNC_BL_RND);
<main_process>"bl_wait"         return(__FUNC_BL_WAIT);
<main_process>"bl_color"        return(__FUNC_BL_COLOR);
<main_process>"bl_locate"       return(__FUNC_BL_LOCATE);
<main_process>"bl_getPix"       return(__FUNC_BL_GETPIX);
<main_process>"bl_waitNF"       return(__FUNC_BL_WAITNF);
<main_process>"bl_inkey1"       return(__FUNC_BL_INKEY1);
<main_process>"bl_cls"          return(__FUNC_BL_CLS);
<main_process>"bl_inptInt"      return(__FUNC_BL_INPTINT);
<main_process>"bl_inptFlot"     return(__FUNC_BL_INPTFLOT);
<main_process>"bl_setMode"      return(__FUNC_BL_SETMODE);
<main_process>"bl_fillOval"     return(__FUNC_BL_FILLOVAL);
<main_process>"bl_drawStr"      return( __FUNC_BL_DRAWSTR);
<main_process>"bl_openVWin"     return(__FUNC_BL_OPENVWIN);
<main_process>"bl_slctWin"      return(__FUNC_BL_SLCTWIN);
<main_process>"bl_copyRct0"     return(__FUNC_BL_COPYRCT0);
<main_process>"bl_copyRct1"     return(__FUNC_BL_COPYRCT1);
<main_process>"bl_drawPtrn_d"   return(__FUNC_BL_DRAWPTRN_D);
<main_process>"bl_drawPtrn_r"   return(__FUNC_BL_DRAWPTRN_R);

<main_process>[_a-zA-Z][_0-9a-zA-Z]* {
        strcpy(yylval.identifier, yytext);
        return(__IDENTIFIER);
}

<main_process>[:][_a-zA-Z][_0-9a-zA-Z]* {
}

<main_process><<EOF>>   return(__EOF);

<main_process_skip_declaration_block>[{] {
        if (skip_declaration_block_depth++ == 0)
                return(__BLOCK_BEGIN);
}

<main_process_skip_declaration_block>[}] {
        if (--skip_declaration_block_depth == 0) {
                BEGIN(main_process);
                return(__BLOCK_END);
        }
}

<main_process_skip_declaration_block>. {
}

%%

void start_create_jmptbl(void)
{
        BEGIN(create_jmptbl);
}

void start_main_process(void)
{
        BEGIN(main_process);
}

void skip_declaration_block(void)
{
        if (strcmp(yytext, "{") == 0)
                skip_declaration_block_depth = 1;
        else
                skip_declaration_block_depth = 0;

        BEGIN(main_process_skip_declaration_block);
}
