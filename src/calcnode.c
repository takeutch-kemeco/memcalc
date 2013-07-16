/* calcnode.c
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
#include <stdbool.h>
#include "mem.h"
#include "node.h"
#include "complex.h"
#include "calcnode.h"
#include "calcnode_expression.h"
#include "calcnode_function.h"
#include "calcnode_comparison.h"
#include "calcnode_read_variable.h"
#include "calcnode_declarator.h"
#include "calcnode_selection.h"
#include "calcnode_lambda.h"
#include "memcalc.bison.h"

static struct CalcNode calcnode__IDENTIFIER(struct Node* a)
{
        const char* name = (char*)node_child(a, 0);

        struct CalcNode cn0 = {.type = CNT_STRPTR, .ptr = (void*)name};

        return cn0;
}

static struct CalcNode calcnode__ASSIGNMENT(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        if (cn0.type != CNT_VARPTR) {
                printf("syntax err: 変数以外へ代入しようとしてます\n");
                exit(1);
        }

        struct MemTag* cn0m = (struct MemTag*)(cn0.ptr);

        struct CalcNode cn1 = calcnode(node_child(a, 1));
        switch (cn1.type) {
        case CNT_NOT_FOUND:
                printf("syntax err: 代入式の右辺が存在しません\n");
                exit(1);

        case CNT_BOTTOM:
                printf("syntax err: 代入式の右辺のこの関数は、戻り値を持たないタイプです\n");
                exit(1);

        case CNT_COMPVAL:
                cn0m->type = MTT_COMPVAL;
                *(((struct Complex*)cn0m->address) + cn0m->index) = cn1.compval;
                break;

        case CNT_FUNCPTR:
                cn0m->type = MTT_FUNCPTR;
                *(((void**)cn0m->address) + cn0m->index) = cn1.ptr;
                break;

        case CNT_STRPTR:
                printf("err: calcnode__ASSIGNMENT(), cn1\n");
                exit(1);

        case CNT_VARPTR:
                cn0m->type = MTT_VARPTR;
                *(((void**)cn0m->address) + cn0m->index) = cn1.ptr;
                break;

        default:
                printf("err: calcnode__ASSIGNMENT(), cn1.type\n");
                exit(1);
        }

        return cn1;
}

static struct CalcNode calcnode__DECLARATION_LIST(struct Node* a)
{
        int i;
        for (i = 0; i < a->child_len; i++)
                calcnode(node_child(a, i));

        struct CalcNode cn_ret = {.type = CNT_BOTTOM};
        return cn_ret;
}

static struct CalcNode calcnode__DECLARATION_BLOCK(struct Node* a)
{
        mem_push_overlide();

        int i;
        for (i = 0; i < a->child_len; i++)
                calcnode(node_child(a, i));

        mem_pop_overlide();

        struct CalcNode cn_ret = {.type = CNT_BOTTOM};
        return cn_ret;
}

static struct CalcNode calcnode__LABEL(struct Node* a)
{
        struct CalcNode cn_ret = {.type = CNT_BOTTOM};
        return cn_ret;
}

struct CalcNode calcnode(struct Node* a)
{
        switch (a->ope) {
        case __IDENTIFIER:              return calcnode__IDENTIFIER(a);
        case __ASSIGNMENT:              return calcnode__ASSIGNMENT(a);
        case __DECLARATION_LIST:        return calcnode__DECLARATION_LIST(a);
        case __DECLARATION_BLOCK:       return calcnode__DECLARATION_BLOCK(a);
        case __LABEL:                   return calcnode__LABEL(a);
        }

        struct CalcNode cn;

        cn = calcnode_read_variable(a);
        if (cn.type != CNT_NOT_FOUND)
                return cn;

        cn = calcnode_declarator(a);
        if (cn.type != CNT_NOT_FOUND)
                return cn;

        cn = calcnode_selection(a);
        if (cn.type != CNT_NOT_FOUND)
                return cn;

        cn = calcnode_lambda(a);
        if (cn.type != CNT_NOT_FOUND)
                return cn;

        cn = calcnode_expression(a);
        if (cn.type != CNT_NOT_FOUND)
                return cn;

        cn = calcnode_function(a);
        if (cn.type != CNT_NOT_FOUND)
                return cn;

        cn = calcnode_comparison(a);
        if (cn.type != CNT_NOT_FOUND)
                return cn;

        cn.type = CNT_NOT_FOUND;
        return cn;
}
