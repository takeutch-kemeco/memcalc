/* calcnode_selection.c
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

static struct CalcNode calcnode__SELECTION_IF_then_else(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        if (cn0.type != CNT_COMPVAL) {
                printf("syntax err: if 文の条件式が不正です\n");
                exit(1);
        }

        if (complex_realpart(cn0.compval) != 0)
                calcnode(node_child(a, 1));
        else
                calcnode(node_child(a, 2));

        cn0.type = CNT_BOTTOM;
        return cn0;
}

static struct CalcNode calcnode__SELECTION_IF_then_only(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        if (cn0.type != CNT_COMPVAL) {
                printf("syntax err: if 文の条件式が不正です\n");
                exit(1);
        }

        if (complex_realpart(cn0.compval) != 0)
                calcnode(node_child(a, 1));

        cn0.type = CNT_BOTTOM;
        return cn0;
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

struct CalcNode calcnode_selection(struct Node* a)
{
        switch (a->ope) {
        case __SELECTION_IF:            return calcnode__SELECTION_IF(a);
        case __SELECTION_EXP:           return calcnode__SELECTION_EXP(a);
        }

        struct CalcNode cn0 = {.type = CNT_NOT_FOUND};
        return cn0;
}
