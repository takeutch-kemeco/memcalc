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
#include "mem.h"
#include "node.h"
#include "complex.h"
#include "calcnode.h"
#include "calcnode_expression.h"
#include "calcnode_function.h"
#include "calcnode_comparison.h"
#include "memcalc.bison.h"

static struct CalcNode calcnode__IDENTIFIER(struct Node* a)
{
        const char* name = (char*)node_child(a, 0);

        struct CalcNode cn0 = {.type = CNT_STRPTR, .ptr = (void*)name};

        return cn0;
}

static struct CalcNode calcnode__DECLARATOR_scalar(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        if (cn0.type != CNT_STRPTR) {
                printf("syntax err: 代入先が変数ではありません\n");
                exit(1);
        }

        const char* name = (char*)(cn0.ptr);
        struct MemTag* var = mem_read_var_memtag(name, MTT_VARPTR, 0);

        cn0.type = CNT_VARPTR;
        cn0.ptr = (void*)var;

        return cn0;
}

static struct CalcNode calcnode__DECLARATOR_array(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        if (cn0.type != CNT_STRPTR) {
                printf("syntax err: 代入先が変数ではありません\n");
                exit(1);
        }

        struct CalcNode cn1 = calcnode(node_child(a, 1));
        if (cn1.type != CNT_COMPVAL) {
                printf("syntax err: 代入先の変数のインデックスが不正です\n");
                exit(1);
        }

        const char* name = (char*)(cn0.ptr);
        const size_t index = complex_realpart(cn1.compval);
        struct MemTag* var = mem_read_var_memtag(name, MTT_VARPTR, index);

        cn0.type = CNT_VARPTR;
        cn0.ptr = (void*)var;

        return cn0;
}

static struct CalcNode calcnode__DECLARATOR(struct Node* a)
{
        switch (a->child_len) {
        case 1: return calcnode__DECLARATOR_scalar(a);
        case 2: return calcnode__DECLARATOR_array(a);
        }

        if (a->child_len == 0) {
                printf("syntax err: 代入先の変数が存在しないか、不正です\n");
                exit(1);
        } else {
                printf("syntac err: 代入先の変数の配列の次元が高すぎます\n");
                exit(1);
        }
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
                *((struct Complex*)(cn0m->address)) = cn1.compval;
                break;

        case CNT_FUNCPTR:
                cn0m->address = cn1.ptr;
                break;

        case CNT_STRPTR:
                printf("err: calcnode__ASSIGNMENT(), cn1\n");
                exit(1);

        case CNT_VARPTR:
                cn0m->address = cn1.ptr;
                break;

        default:
                printf("err: calcnode__ASSIGNMENT(), cn1.type\n");
                exit(1);
        }

        return cn1;
}

static struct CalcNode calcnode__SELECTION_IF_then_else(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        if (cn0.type != CNT_COMPVAL) {
                printf("syntax err: if 文の条件式が不正です\n");
                exit(1);
        }

        struct CalcNode cnx = {.type = CNT_FUNCPTR};
        if (complex_realpart(cn0.compval) != 0)
                cnx.ptr = (void*)node_child(a, 1);
        else
                cnx.ptr = (void*)node_child(a, 2);

        return cnx;
}

static struct CalcNode calcnode__SELECTION_IF_then_only(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        if (cn0.type != CNT_COMPVAL) {
                printf("syntax err: if 文の条件式が不正です\n");
                exit(1);
        }

        struct CalcNode cnx;
        if (complex_realpart(cn0.compval) != 0) {
                cnx.type = CNT_FUNCPTR;
                cnx.ptr = (void*)node_child(a, 1);
        } else {
                cnx.type = CNT_BOTTOM;
        }

        return cnx;
}

static struct CalcNode calcnode__SELECTION_IF(struct Node* a)
{
        switch (a->child_len) {
        case 2:         return calcnode__SELECTION_IF_then_only(a);
        case 3:         return calcnode__SELECTION_IF_then_else(a);
        }

        printf("syntax err: if 文の分岐先の数が不正です\n");
        exit(1);
}

static struct CalcNode calcnode__SELECTION_EXP(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        if (cn0.type != CNT_COMPVAL) {
                printf("syntax err: (条件式) -> a :: b の条件式が不正です\n");
                exit(1);
        }

        struct CalcNode cnx;
        if (complex_realpart(cn0.compval) != 0)
                cnx = calcnode(node_child(a, 1));
        else
                cnx = calcnode(node_child(a, 2));

        return cnx;
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

static int xxx = 0;

struct CalcNode calcnode(struct Node* a)
{
        printf("node address:[%p], xxx:[%d]\n", (void*)a, xxx++);

        switch (a->ope) {
        case __IDENTIFIER:              return calcnode__IDENTIFIER(a);
        case __DECLARATOR:              return calcnode__DECLARATOR(a);
        case __ASSIGNMENT:              return calcnode__ASSIGNMENT(a);
        case __SELECTION_IF:            return calcnode__SELECTION_IF(a);
        case __SELECTION_EXP:           return calcnode__SELECTION_EXP(a);
        case __DECLARATION_LIST:        return calcnode__DECLARATION_LIST(a);
        case __DECLARATION_BLOCK:       return calcnode__DECLARATION_BLOCK(a);
        case __LABEL:                   return calcnode__LABEL(a);
        }

        struct CalcNode cn;

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
