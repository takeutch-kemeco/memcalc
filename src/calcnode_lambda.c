/* calcnode_lambda.c
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

static struct CalcNode calcnode__LAMBDA_ABSTRACT(struct Node* a)
{
        struct CalcNode cn_ret = {
                .type = CNT_FUNCPTR,
                .ptr = (void*)a,
        };

        return cn_ret;
}

static struct CalcNode calcnode_calc_lambda(struct Node* f0, struct Node* n0)
{
        if (f0->ope != __LAMBDA_ABSTRACT) {
                printf("err: calcnode.c, calcnode_calc_lambda()\n");
                exit(1);
        }

        struct Node* tmp = node_new(__ASSIGNMENT);
        node_link(tmp, node_child(f0, 0));
        node_link(tmp, n0);
        calcnode(tmp);
        /* node_delete_shallow(tmp); */

        return calcnode(node_child(f0, 1));
}

static struct CalcNode calcnode__FUNCTION_DESCRIPTION(struct Node* a)
{
        struct Node* n0 = node_child(a, 0);
        struct Node* n1 = node_child(a, 1);

        struct CalcNode cn_ret;

        struct CalcNode cn0 = calcnode(n0);
        if (cn0.type == CNT_FUNCPTR) {
                mem_push_overlide();

                struct Node* f0 = (struct Node*)(cn0.ptr);
                cn_ret = calcnode_calc_lambda(f0, n1);

                mem_pop_overlide();
        } else {
                printf("syntax err: 関数以外へ引数を渡そうとしています\n");
                exit(1);
        }

        return cn_ret;
}

struct CalcNode calcnode_lambda(struct Node* a)
{
        switch (a->ope) {
        case __LAMBDA_ABSTRACT:         return calcnode__LAMBDA_ABSTRACT(a);
        case __FUNCTION_DESCRIPTION:    return calcnode__FUNCTION_DESCRIPTION(a);
        }

        struct CalcNode cn0 = {.type = CNT_NOT_FOUND};
        return cn0;
}
