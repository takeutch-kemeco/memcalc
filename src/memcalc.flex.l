/* memcalc.flex.l
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "mem.h"
#include "jmptbl.h"
#include "memcalc.bison.h"

int yycurline = 1;

int yycurbyte = 0;
int yynextbyte = 0;

static void update_yycurbyte(void)
{
        yycurbyte = yynextbyte;
        yynextbyte += yyleng;
#ifdef DEBUG
        printf("[%s, %d, %d, %d]\n", yytext, yycurline, yycurbyte, yynextbyte);
#endif /* DEBUG */
}

static void update_line_yycurbyte(void)
{
        yycurbyte = yynextbyte;
        yynextbyte += yyleng;
        yycurline++;
#ifdef DEBUG
        printf("[%s, %d, %d, %d]\n", yytext, yycurline, yycurbyte, yynextbyte);
#endif /* DEBUG */
}

#define update_return(a) {update_yycurbyte(); return a;} while(0)

#define update_line_return(a) {update_line_yycurbyte(); return a;} while(0)

%}

%x pre_process
%x main_process
%x main_process_comment
%x main_process_char

%%

<pre_process>. {
}

<main_process>[/][*] {
        update_yycurbyte();

        /* c スタイルのコメントの開始点 */
        BEGIN(main_process_comment);
}

<main_process_comment>[*][/] {
        update_yycurbyte();

        /* c スタイルのコメントの終了点 */
        BEGIN(main_process);
}

<main_process_comment>[\n] {
        update_line_yycurbyte();

        /* c スタイルのコメントの改行 */
}

<main_process_comment>. {
        update_yycurbyte();

        /* c スタイルのコメントの本文 */
}

<main_process>"//".*[\n] {
        update_line_yycurbyte();

        /* c++ スタイルのコメント */
}

<main_process>['] {
        update_yycurbyte();

        /* 一文字リテラルの開始点 */
        BEGIN(main_process_char);
}

<main_process_char>['] {
        update_yycurbyte();

        /* 一文字リテラルの終了点 */
        BEGIN(main_process);
}

<main_process_char>[\\][n] {
        yylval.realval = '\n';
        update_return(__CONST_FLOAT);
}

<main_process_char>[\\][0] {
        yylval.realval = '\0';
        update_return(__CONST_FLOAT);
}

<main_process_char>[\\][\\] {
        yylval.realval = '\\';
        update_return(__CONST_FLOAT);
}

<main_process_char>[\\]['] {
        yylval.realval = '\'';
        update_return(__CONST_FLOAT);
}

<main_process_char>[\\]["] {
        yylval.realval = '\"';
        update_return(__CONST_FLOAT);
}

<main_process_char>[\n] {
        yylval.realval = yytext[0];
        update_line_return(__CONST_FLOAT);
}

<main_process_char>. {
        yylval.realval = yytext[0];
        update_return(__CONST_FLOAT);
}

<main_process>[0][0-9]+[.]?[0-9]* {
        printf("\n%s\n構文エラー : 数値の先頭に不適切な 0 があります\n\n", yytext);
        exit(1);
}

<main_process>[0-9]+[.]?[0-9]* {
        yylval.realval = atof(yytext);
        update_return(__CONST_FLOAT);
}

<main_process>[0][.][0-9]* {
        yylval.realval = atof(yytext);
        update_return(__CONST_FLOAT);
}

<main_process>":+"      update_return(__OPE_COMPLEX);
<main_process>":<"      update_return(__OPE_COMPLEX_POLAR);
<main_process>"Re"      update_return(__OPE_REAL_PART);
<main_process>"Im"      update_return(__OPE_IMAGINARY_PART);
<main_process>"~"       update_return(__OPE_CONJUGATE);
<main_process>"|"       update_return(__OPE_ABSOLUTE);
<main_process>"arg"     update_return(__OPE_ARGUMENT);
<main_process>"^"       update_return(__OPE_POWER);

<main_process>"+"       update_return(__OPE_ADD);
<main_process>"-"       update_return(__OPE_SUB);
<main_process>"*"       update_return(__OPE_MUL);
<main_process>"/"       update_return(__OPE_DIV);
<main_process>"%"       update_return(__OPE_MOD);

<main_process>"<<"      update_return(__OPE_LSHIFT);
<main_process>">>"      update_return(__OPE_RSHIFT);

<main_process>"or"      update_return(__OPE_OR);
<main_process>"and"     update_return(__OPE_AND);
<main_process>"xor"     update_return(__OPE_XOR);
<main_process>"not"     update_return(__OPE_NOT);

<main_process>"=="      update_return(__OPE_COMPARISON);
<main_process>"/="      update_return(__OPE_NOT_COMPARISON);
<main_process>"<"       update_return(__OPE_ISSMALL);
<main_process>"<="      update_return(__OPE_ISSMALL_COMP);
<main_process>">"       update_return(__OPE_ISLARGE);
<main_process>">="      update_return(__OPE_ISLARGE_COMP);

<main_process>"="       update_return(__OPE_SUBST);

<main_process>"("       update_return(__LB);
<main_process>")"       update_return(__RB);

<main_process>"{"       update_return(__BLOCK_BEGIN);
<main_process>"}"       update_return(__BLOCK_END);

<main_process>[\\]      update_return(__BACKSLASH)
<main_process>"."       update_return(__COLON);
<main_process>","       update_return(__CAMMA);
<main_process>";"       update_return(__DECL_END);

<main_process>"if"      update_return(__STATE_IF);
<main_process>"else"    update_return(__STATE_ELSE);

<main_process>"->"      update_return(__STATE_EXP_IF);
<main_process>"::"     update_return(__STATE_EXP_ELSE);

<main_process>"["       update_return(__ARRAY_BEGIN);
<main_process>"]"       update_return(__ARRAY_END);

<main_process>"goto"    update_return(__OPE_GOTO);
<main_process>"gosub"   update_return(__OPE_GOSUB);
<main_process>"return"  update_return(__OPE_RETURN);

<main_process>"print"           update_return(__FUNC_PRINT);
<main_process>"putpixel"        update_return(__FUNC_PUTPIXEL);
<main_process>"putchar"         update_return(__FUNC_PUTCHAR);

<main_process>"bl_putc"         update_return(__FUNC_BL_PUTC);
<main_process>"bl_puts1"        update_return(__FUNC_BL_PUTS1);
<main_process>"bl_printf"       update_return(__FUNC_BL_PRINTF);
<main_process>"bl_scanf"        update_return(__FUNC_BL_SCANF);
<main_process>"bl_malloc"       update_return(__FUNC_BL_MALLOC);
<main_process>"bl_rand"         update_return(__FUNC_BL_RAND);
<main_process>"bl_srand"        update_return(__FUNC_BL_SRAND);
<main_process>"bl_gets"         update_return(__FUNC_BL_GETS);
<main_process>"bl_openWin"      update_return(__FUNC_BL_OPENWIN);
<main_process>"bl_setCol"       update_return(__FUNC_BL_SETCOL);
<main_process>"bl_setBCol"      update_return(__FUNC_BL_SETBCOL);
<main_process>"bl_rgb"          update_return(__FUNC_BL_RGB);
<main_process>"bl_iCol"         update_return(__FUNC_BL_ICOL);
<main_process>"bl_flshWin"      update_return(__FUNC_BL_FLSHWIN);
<main_process>"bl_getGrpB"      update_return(__FUNC_BL_GETGRPB);
<main_process>"bl_setPix"       update_return(__FUNC_BL_SETPIX);
<main_process>"bl_fillRect"     update_return(__FUNC_BL_FILLRECT);
<main_process>"bl_drawRect"     update_return(__FUNC_BL_DRAWRECT);
<main_process>"bl_drawLine"     update_return(__FUNC_BL_DRAWLINE);
<main_process>"bl_rnd"          update_return(__FUNC_BL_RND);
<main_process>"bl_wait"         update_return(__FUNC_BL_WAIT);
<main_process>"bl_color"        update_return(__FUNC_BL_COLOR);
<main_process>"bl_locate"       update_return(__FUNC_BL_LOCATE);
<main_process>"bl_getPix"       update_return(__FUNC_BL_GETPIX);
<main_process>"bl_waitNF"       update_return(__FUNC_BL_WAITNF);
<main_process>"bl_inkey1"       update_return(__FUNC_BL_INKEY1);
<main_process>"bl_cls"          update_return(__FUNC_BL_CLS);
<main_process>"bl_inptInt"      update_return(__FUNC_BL_INPTINT);
<main_process>"bl_inptFlot"     update_return(__FUNC_BL_INPTFLOT);
<main_process>"bl_setMode"      update_return(__FUNC_BL_SETMODE);
<main_process>"bl_fillOval"     update_return(__FUNC_BL_FILLOVAL);
<main_process>"bl_drawStr"      update_return( __FUNC_BL_DRAWSTR);
<main_process>"bl_openVWin"     update_return(__FUNC_BL_OPENVWIN);
<main_process>"bl_slctWin"      update_return(__FUNC_BL_SLCTWIN);
<main_process>"bl_copyRct0"     update_return(__FUNC_BL_COPYRCT0);
<main_process>"bl_copyRct1"     update_return(__FUNC_BL_COPYRCT1);
<main_process>"bl_drawPtrn_d"   update_return(__FUNC_BL_DRAWPTRN_D);
<main_process>"bl_drawPtrn_r"   update_return(__FUNC_BL_DRAWPTRN_R);

<main_process>[_a-zA-Z][_0-9a-zA-Z]* {
        strcpy(yylval.identifier_str, yytext);
        update_return(__IDENTIFIER);
}

<main_process>[:][_a-zA-Z][_0-9a-zA-Z]* {
        update_yycurbyte();
}

<main_process>[\n] {
        update_line_yycurbyte();
}

<main_process>. {
        update_yycurbyte();
}

<main_process><<EOF>>   return(__EOF);

%%

void start_pre_process(void)
{
        BEGIN(pre_process);
}

void start_main_process(void)
{
        BEGIN(main_process);
}
