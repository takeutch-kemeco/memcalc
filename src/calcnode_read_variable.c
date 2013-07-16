/* calcnode_read_variable.c
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

static struct CalcNode calcnode__READ_VARIABLE_work(struct MemTag* mt, const size_t index)
{
        struct CalcNode cn0;

        switch (mt->type) {
        case MTT_COMPVAL:
                cn0.type = CNT_COMPVAL;
                cn0.compval = *(((struct Complex*)(mt->address)) + index);
                break;

        case MTT_VARPTR:
                cn0.type = CNT_VARPTR;
                cn0.ptr = *(((void**)(mt->address)) + index);
                break;

        case MTT_FUNCPTR:
                cn0.type = CNT_FUNCPTR;
                cn0.ptr = *(((void**)(mt->address)) + index);
                break;
        }

        return cn0;
}

static struct CalcNode calcnode__READ_VARIABLE_scalar(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        if (cn0.type != CNT_STRPTR) {
                printf("err: calcnode.c, calcnode__READ_VARIABLE_scalar()\n");
                exit(1);
        }

        const char* name = (char*)(cn0.ptr);
        const size_t index = 0;
        struct MemTag* mt = mem_read_var_memtag(name, index);

        return calcnode__READ_VARIABLE_work(mt, index);
}

static struct CalcNode calcnode__READ_VARIABLE_array(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        if (cn0.type != CNT_STRPTR) {
                printf("err: calcnode.c, calcnode__READ_VARIABLE_array()\n");
                exit(1);
        }

        struct CalcNode cn1 = calcnode(node_child(a, 1));
        if (cn1.type != CNT_COMPVAL) {
                printf("syntax err: 変数のインデックスが不正です\n");
                exit(1);
        }

        const char* name = (char*)(cn0.ptr);
        const size_t index = complex_realpart(cn1.compval);
        struct MemTag* mt = mem_read_var_memtag(name, index);

        return calcnode__READ_VARIABLE_work(mt, index);
}

static struct CalcNode calcnode__READ_VARIABLE(struct Node* a)
{
        switch (a->child_len) {
        case 1: return calcnode__READ_VARIABLE_scalar(a);
        case 2: return calcnode__READ_VARIABLE_array(a);
        }

        if (a->child_len == 0) {
                printf("err: calcnode.c, calcnode__READ_VARIABLE_array()\n");
                exit(1);
        } else {
                printf("syntac err: 配列の次元が不正です\n");
                exit(1);
        }
}

struct CalcNode calcnode_read_variable(struct Node* a)
{
        switch (a->ope) {
        case __READ_VARIABLE:           return calcnode__READ_VARIABLE(a);
        }

        struct CalcNode cn0 = {.type = CNT_NOT_FOUND};
        return cn0;
}
