/* test_calcnode_expression.c
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

#include <blike0.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "complex.h"
#include "node.h"
#include "calcnode.h"
#include "calcnode_expression.h"
#include "memcalc.bison.h"

struct Node* t(void)
{
        struct Node* o = node_new(__OPE_ADD);

        struct Complex* lval = complex_new(1, 2);
        struct Node* l = node_new_leaf(__CONST_FLOAT, lval);

        struct Complex* rval = complex_new(3, 4);
        struct Node* r = node_new_leaf(__CONST_FLOAT, rval);

        node_link(o, l);
        node_link(o, r);

        return o;
}

void f(void)
{
        struct Node* a = t();
        struct CalcNode cn = calcnode(a);
        printf("%f, %f\n", cn.compval.re, cn.compval.im);
}


blMain()
{
        f();
}
