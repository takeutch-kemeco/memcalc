/* node_debug.c
 * Copyright (C) 2013 Takeutch Kemeco
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "complex.h"
#include "node.h"
#include "memcalc.bison.h"

static char* token_to_str(const uint64_t token)
{
        switch (token) {
        case __FUNC_BL_PUTC: return "__FUNC_BL_PUTC";
        case __FUNC_BL_PUTS1: return "__FUNC_BL_PUTS1";
        case __FUNC_BL_PRINTF: return "__FUNC_BL_PRINTF";
        case __FUNC_BL_SCANF: return "__FUNC_BL_SCANF";
        case __FUNC_BL_MALLOC: return "__FUNC_BL_MALLOC";
        case __FUNC_BL_RAND: return "__FUNC_BL_RAND";
        case __FUNC_BL_SRAND: return "__FUNC_BL_SRAND";
        case __FUNC_BL_GETS: return "__FUNC_BL_GETS";
        case __FUNC_BL_OPENWIN: return "__FUNC_BL_OPENWIN";
        case __FUNC_BL_SETCOL: return "__FUNC_BL_SETCOL";
        case __FUNC_BL_SETBCOL: return "__FUNC_BL_SETBCOL";
        case __FUNC_BL_RGB: return "__FUNC_BL_RGB";
        case __FUNC_BL_ICOL: return "__FUNC_BL_ICOL";
        case __FUNC_BL_FLSHWIN: return "__FUNC_BL_FLSHWIN";
        case __FUNC_BL_GETGRPB: return "__FUNC_BL_GETGRPB";
        case __FUNC_BL_SETPIX: return "__FUNC_BL_SETPIX";
        case __FUNC_BL_FILLRECT: return "__FUNC_BL_FILLRECT";
        case __FUNC_BL_DRAWRECT: return "__FUNC_BL_DRAWRECT";
        case __FUNC_BL_DRAWLINE: return "__FUNC_BL_DRAWLINE";
        case __FUNC_BL_RND: return "__FUNC_BL_RND";
        case __FUNC_BL_WAIT: return "__FUNC_BL_WAIT";
        case __FUNC_BL_COLOR: return "__FUNC_BL_COLOR";
        case __FUNC_BL_LOCATE: return "__FUNC_BL_LOCATE";
        case __FUNC_BL_GETPIX: return "__FUNC_BL_GETPIX";
        case __FUNC_BL_WAITNF: return "__FUNC_BL_WAITNF";
        case __FUNC_BL_INKEY1: return "__FUNC_BL_INKEY1";
        case __FUNC_BL_CLS: return "__FUNC_BL_CLS";
        case __FUNC_BL_INPTINT: return "__FUNC_BL_INPTINT";
        case __FUNC_BL_INPTFLOT: return "__FUNC_BL_INPTFLOT";
        case __FUNC_BL_SETMODE: return "__FUNC_BL_SETMODE";
        case __FUNC_BL_FILLOVAL: return "__FUNC_BL_FILLOVAL";
        case __FUNC_BL_DRAWSTR: return "__FUNC_BL_DRAWSTR";
        case __FUNC_BL_OPENVWIN: return "__FUNC_BL_OPENVWIN";
        case __FUNC_BL_SLCTWIN: return "__FUNC_BL_SLCTWIN";
        case __FUNC_BL_COPYRCT0: return "__FUNC_BL_COPYRCT0";
        case __FUNC_BL_COPYRCT1: return "__FUNC_BL_COPYRCT1";
        case __FUNC_BL_DRAWPTRN_D: return "__FUNC_BL_DRAWPTRN_D";
        case __FUNC_BL_DRAWPTRN_R: return "__FUNC_BL_DRAWPTRN_R";
        case __FUNC_PRINT: return "__FUNC_PRINT";
        case __FUNC_PUTPIXEL: return "__FUNC_PUTPIXEL";
        case __FUNC_PUTCHAR: return "__FUNC_PUTCHAR";
        case __STATE_IF: return "__STATE_IF";
        case __STATE_ELSE: return "__STATE_ELSE";
        case __STATE_EXP_IF: return "__STATE_EXP_IF";
        case __STATE_EXP_ELSE: return "__STATE_EXP_ELSE";
        case __OPE_GOTO: return "__OPE_GOTO";
        case __OPE_GOSUB: return "__OPE_GOSUB";
        case __OPE_RETURN: return "__OPE_RETURN";
        case __OPE_LABEL: return "__OPE_LABEL";
        case __CONST_FLOAT: return "__CONST_FLOAT";
        case __OPE_PLUS: return "__OPE_PLUS";
        case __OPE_MINUS: return "__OPE_MINUS";
        case __OPE_MUL: return "__OPE_MUL";
        case __OPE_DIV: return "__OPE_DIV";
        case __OPE_MOD: return "__OPE_MOD";
        case __OPE_LSHIFT: return "__OPE_LSHIFT";
        case __OPE_RSHIFT: return "__OPE_RSHIFT";
        case __OPE_OR: return "__OPE_OR";
        case __OPE_AND: return "__OPE_AND";
        case __OPE_XOR: return "__OPE_XOR";
        case __OPE_NOT: return "__OPE_NOT";
        case __OPE_COMPARISON: return "__OPE_COMPARISON";
        case __OPE_NOT_COMPARISON: return "__OPE_NOT_COMPARISON";
        case __OPE_ISSMALL: return "__OPE_ISSMALL";
        case __OPE_ISSMALL_COMP: return "__OPE_ISSMALL_COMP";
        case __OPE_ISLARGE: return "__OPE_ISLARGE";
        case __OPE_ISLARGE_COMP: return "__OPE_ISLARGE_COMP";
        case __OPE_SUBST: return "__OPE_SUBST";
        case __LB: return "__LB";
        case __RB: return "__RB";
        case __ARRAY_BEGIN: return "__ARRAY_BEGIN";
        case __ARRAY_END: return "__ARRAY_END";
        case __BLOCK_BEGIN: return "__BLOCK_BEGIN";
        case __BLOCK_END: return "__BLOCK_END";
        case __CAMMA: return "__CAMMA";
        case __COLON: return "__COLON";
        case __BACKSLASH: return "__BACKSLASH";
        case __DECL_END: return "__DECL_END";
        case __EOF: return "__EOF";
        case __EXPRESSION_LIST: return "__EXPRESSION_LIST";
        case __IDENTIFIER: return "__IDENTIFIER";
        case __DECLARATOR: return "__DECLARATOR";
        case __ASSIGNMENT: return "__ASSIGNMENT";
        case __READ_VARIABLE: return "__READ_VARIABLE";
        case __COMPARISON: return "__COMPARISON";
        case __COMPARISON_UNIT_LIST: return "__COMPARISON_UNIT_LIST";
        case __SELECTION_IF: return "__SELECTION_IF";
        case __SELECTION_EXP: return "__SELECTION_EXP";
        case __DECLARATION_LIST: return "__DECLARATION_LIST";
        case __DECLARATION_BLOCK: return "__DECLARATION_BLOCK";
        case __GOTO: return "__GOTO";
        case __GOSUB: return "__GOSUB";
        case __RETURN: return "__RETURN";
        case __LABEL: return "__LABEL";
        case __LAMBDA_ABSTRACT: return "__LAMBDA_ABSTRACT";
        case __FUNCTION_DESCRIPTION: return "__FUNCTION_DESCRIPTION";
        case __OPE_SUB: return "__OPE_SUB";
        case __OPE_ADD: return "__OPE_ADD";
        case __OPE_COMPLEX_POLAR: return "__OPE_COMPLEX_POLAR";
        case __OPE_COMPLEX: return "__OPE_COMPLEX";
        case __OPE_POWER: return "__OPE_POWER";
        case __OPE_ARGUMENT: return "__OPE_ARGUMENT";
        case __OPE_CONJUGATE: return "__OPE_CONJUGATE";
        case __OPE_ABSOLUTE: return "__OPE_ABSOLUTE";
        case __OPE_IMAGINARY_PART: return "__OPE_IMAGINARY_PART";
        case __OPE_REAL_PART: return "__OPE_REAL_PART";
        default:
                printf("err: [%d] is undefined token\n", token);
        }
}

void node_print_tree(struct Node* a)
{
        printf("\nnode_print_tree(), Root:[%p]\n\n", (void*)a);
        static int indent = 0;

        void __node_print(struct Node* a)
        {
                int i;
                for (i = 0; i < indent; i++)
                        printf("  ");

                printf("<ptr:[%p], ope:[%s], len:[%d]", (void*)a, token_to_str(a->ope), a->child_len);

                switch (a->ope) {
                case __CONST_FLOAT:
                        {
                                struct Complex tmp = *((struct Complex*)node_child(a, 0));
                                printf(", value:[%f, %f]>\n",
                                       complex_realpart(tmp), complex_imagpart(tmp));
                        }
                        break;

                case __DECL_END:
                        printf(">\n");
                        break;

                case __LABEL:
                        printf(", value:[%s]>\n", (char*)node_child(a, 0));
                        break;

                case __IDENTIFIER:
                        printf(", value:[%s]>\n", (char*)node_child(a, 0));
                        break;

                case __DECLARATOR:
                        {
                                char* s = (char*)node_child(a, 0);

                                switch (a->child_len) {
                                case 1:
                                        printf(", value:[%s, [%f, %f]]>\n", s);
                                        break;

                                case 2:
                                        {
                                                struct Complex c = *((struct Complex*)node_child(a, 1));
                                                printf(", value:[%s, [%f, %f]]>\n",
                                                       s, complex_realpart(c), complex_imagpart(c));
                                        }
                                        break;

                                default:
                                        printf("err: __DECLARATOR, child_len\n");
                                        exit(1);
                                }
                        }
                        break;

                default:
                        printf("\n");

                        indent++;

                        for (i = 0; i < a->child_len; i++) {
                                __node_print(node_child(a, i));
                        }

                        indent--;
                }
        }

        __node_print(a);
}
