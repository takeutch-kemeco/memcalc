/* calcnode_function.c
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
#include "calcnode.h"
#include "calcnode_function.h"
#include "calcnode_bl_function.h"
#include "func_putpixel.h"
#include "memcalc.bison.h"

static struct CalcNode calcnode_function__FUNC_PRINT(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        const double re = complex_realpart(cn0.compval);
        const double im = complex_imagpart(cn0.compval);

        if (im >= 0.0)
                printf("%f+%fi\n", re, im);
        else
                printf("%f%fi\n", re, im);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_function__FUNC_PUTPIXEL(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));
        struct CalcNode cn2 = calcnode(node_child(a, 2));
        struct CalcNode cn3 = calcnode(node_child(a, 3));
        struct CalcNode cn4 = calcnode(node_child(a, 4));

        const double cn0re = complex_realpart(cn0.compval);
        const double cn1re = complex_realpart(cn1.compval);
        const double cn2re = complex_realpart(cn2.compval);
        const double cn3re = complex_realpart(cn3.compval);
        const double cn4re = complex_realpart(cn4.compval);

        __func_putpixel(cn0re, cn1re, cn2re, cn3re, cn4re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_function__FUNC_PUTCHAR(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        putchar(complex_realpart(cn0.compval));

        cn0.type = CNT_BOTTOM;

        return cn0;
}

struct CalcNode calcnode_function(struct Node* a)
{
        switch (a->ope) {
        case __FUNC_PRINT:           return calcnode_function__FUNC_PRINT(a);
        case __FUNC_PUTPIXEL:        return calcnode_function__FUNC_PUTPIXEL(a);
        case __FUNC_PUTCHAR:         return calcnode_function__FUNC_PUTCHAR(a);
        }

        return calcnode_bl_function(a);
}
