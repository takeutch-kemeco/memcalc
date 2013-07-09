/* calcnode_comparison.c
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
#include "memcalc.bison.h"

static struct Complex calcnode_complex_comparison(const uint64_t ope,
                                                  struct Complex a,
                                                  struct Complex b)
{
        switch (ope) {
        case __OPE_COMPARISON:          return complex_comparison(a, b);
        case __OPE_NOT_COMPARISON:      return complex_not_comparison(a, b);
        case __OPE_ISSMALL:             return complex_is_small(a, b);
        case __OPE_ISSMALL_COMP:        return complex_is_small_comp(a, b);
        case __OPE_ISLARGE:             return complex_is_large(a, b);
        case __OPE_ISLARGE_COMP:        return complex_is_large_comp(a, b);
        }

        printf("err: calcnode_comparison.c, calcnode_complex_comparison()\n");
        exit(1);
}

static struct CalcNode calcnode__COMPARISON_UNIT_LIST(struct Node* a)
{
        struct Node* n0 = node_child(a, 0);
        struct CalcNode cn0 = calcnode(n0);
        if (cn0.type != CNT_COMPVAL) {
                printf("syntax err: 比較を行えるのは複素数型のみです\n");
                exit(1);
        }

        struct Node* n1 = node_child(a, 1);
        struct CalcNode cn1 = calcnode(n1);
        if (cn0.type != CNT_COMPVAL) {
                printf("syntax err: 比較を行えるのは複素数型のみです\n");
                exit(1);
        }

        cn0.comparison_val = calcnode_complex_comparison(cn1.comparison_ope, cn0.compval, cn1.compval);
        cn0.comparison_val = complex_and(cn0.comparison_val, cn1.comparison_val);

        return cn0;
}

static struct CalcNode calcnode__COMPARISON(struct Node* a)
{
        struct CalcNode cn0 = calcnode__COMPARISON_UNIT_LIST(a);

        cn0.compval = cn0.comparison_val;

        return cn0;
}

static struct CalcNode calcnode__OPE_X(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        cn0.comparison_val = complex_constructor(1, 1);
        cn0.comparison_ope = a->ope;

        return cn0;
}

struct CalcNode calcnode_comparison(struct Node* a)
{
        switch (a->ope) {
        case __COMPARISON:              return calcnode__COMPARISON(a);
        case __COMPARISON_UNIT_LIST:    return calcnode__COMPARISON_UNIT_LIST(a);

        case __OPE_COMPARISON:
        case __OPE_NOT_COMPARISON:
        case __OPE_ISSMALL:
        case __OPE_ISSMALL_COMP:
        case __OPE_ISLARGE:
        case __OPE_ISLARGE_COMP:        return calcnode__OPE_X(a);
        }

        struct CalcNode cn0 = {.type = CNT_NOT_FOUND};
        return cn0;
}
