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
#include "mem.h"
#include "jmptbl.h"
#include "memcalc.bison.h"

long create_jmptbl_ccount;

%}

%x create_jmptbl
%x main_process
%x main_process_comment

%%

<create_jmptbl>[_a-zA-Z][_0-9a-zA-Z]*[:] {
        char* tmp = yytext;
        tmp[yyleng - 1] = '\0';

        create_jmptbl_ccount += yyleng;
        jmptbl_add(tmp, create_jmptbl_ccount);
}

<create_jmptbl>.|[\n] {
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

<main_process>[0][0-9]+[.]?[0-9]* {
        printf("\n%s\n構文エラー : 数値の先頭に不適切な 0 があります\n\n", yytext);
        exit(1);
}

<main_process>[0-9]+[.]?[0-9]* {
        yylval.val = atof(yytext);
        return(__CONST_FLOAT);
}

<main_process>[0][.][0-9]* {
        yylval.val = atof(yytext);
        return(__CONST_FLOAT);
}

<main_process>"+"       return(__OPE_ADD);
<main_process>"-"       return(__OPE_SUB);
<main_process>"*"       return(__OPE_MUL);
<main_process>"/"       return(__OPE_DIV);
<main_process>"%"       return(__OPE_MOD);

<main_process>"<<"      return(__OPE_LSHIFT);
<main_process>">>"      return(__OPE_RSHIFT);

<main_process>"|"       return(__OPE_OR);
<main_process>"&"       return(__OPE_AND);
<main_process>"^"       return(__OPE_XOR);
<main_process>"~"       return(__OPE_NOT);

<main_process>"=="      return(__OPE_COMPARISON);
<main_process>"!="      return(__OPE_NOT_COMPARISON);
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
<main_process>"then"    return(__STATE_THEN);
<main_process>"else"    return(__STATE_ELSE);

<main_process>"goto"    return(__STATE_GOTO);

<main_process>"print"   return(__FUNC_PRINT);
<main_process>"putpixel"        return(__FUNC_PUTPIXEL);

<main_process>[_a-zA-Z][_0-9a-zA-Z]* {
        strcpy(yylval.identifier, yytext);
        return(__IDENTIFIER);
}

<main_process>[_a-zA-Z][_0-9a-zA-Z]*[:] {
}

%%

start_create_jmptbl() {BEGIN(create_jmptbl);}
start_main_process() {BEGIN(main_process);}
