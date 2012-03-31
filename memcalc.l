%{

#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "jmptbl.h"
#include "memcalc.tab.h"

long create_jmptbl_ccount;

%}

%x create_jmptbl
%x main_process

%%

<create_jmptbl>[_a-zA-Z][_0-9a-zA-Z]*[:] {
	char* tmp = yytext;
	tmp[yyleng - 1] = '\0';

	create_jmptbl_ccount += yyleng;
printf("create_jmptbl_ccount[%d]\n", create_jmptbl_ccount);
	jmptbl_add(tmp, create_jmptbl_ccount);
}

<create_jmptbl>.|[\n] {
	create_jmptbl_ccount += yyleng;
}

<main_process>[0-9]+[.]?[0-9]* {
	yylval.val = atof(yytext);
	return(__CONST_FLOAT);
}

<main_process>"+"	return(__OPE_ADD);
<main_process>"-"	return(__OPE_SUB);
<main_process>"*"	return(__OPE_MUL);
<main_process>"/"	return(__OPE_DIV);
<main_process>"%"	return(__OPE_MOD);

<main_process>"<<"	return(__OPE_LSHIFT);
<main_process>">>"	return(__OPE_RSHIFT);

<main_process>"|"	return(__OPE_OR);
<main_process>"&"	return(__OPE_AND);
<main_process>"^"	return(__OPE_XOR);
<main_process>"~"	return(__OPE_NOT);

<main_process>"=="	return(__OPE_COMPARISON);
<main_process>"="	return(__OPE_SUBST);

<main_process>"("	return(__LB);
<main_process>")"	return(__RB);

<main_process>";"	return(__DECL_END);

<main_process>"if"	return(__STATE_IF);
<main_process>"then"	return(__STATE_THEN);
<main_process>"else"	return(__STATE_ELSE);

<main_process>"goto"	return(__STATE_GOTO);

<main_process>[_a-zA-Z][_0-9a-zA-Z]* {
	strcpy(yylval.identifier, yytext);
	return(__IDENTIFIER);
}

<main_process>[_a-zA-Z][_0-9a-zA-Z]*[:] {
}

%%

start_create_jmptbl() {BEGIN(create_jmptbl);}
start_main_process() {BEGIN(main_process);}
