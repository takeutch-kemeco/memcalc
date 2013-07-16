/* calcnode_declarator.c
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
#include "mem.h"
#include "node.h"
#include "complex.h"
#include "calcnode.h"
#include "memcalc.bison.h"

static struct CalcNode calcnode__DECLARATOR_scalar(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        if (cn0.type != CNT_STRPTR) {
                printf("syntax err: 代入先が変数ではありません\n");
                exit(1);
        }

        const char* name = (char*)(cn0.ptr);
        mem_create_var(name, MTT_COMPVAL, 0);
        struct MemTag* var = mem_read_var_memtag(name, 0);

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
        mem_create_var(name, MTT_COMPVAL, index);
        struct MemTag* var = mem_read_var_memtag(name, index);

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

struct CalcNode calcnode_declarator(struct Node* a)
{
        switch (a->ope) {
        case __DECLARATOR:              return calcnode__DECLARATOR(a);
        }

        struct CalcNode cn0 = {.type = CNT_NOT_FOUND};
        return cn0;
}
