/* test_calcnode_function.c
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

#include <blike.h>
#include <stdint.h>
#include "complex.h"
#include "node.h"
#include "calcnode.h"
#include "calcnode_function.h"
#include "memcalc.bison.h"

struct Node* create_print(void)
{
        struct Node* f0 = node_new(__FUNC_PRINT);

        struct Complex* v0 = complex_new(123, 0);
        struct Node* n0 = node_new_leaf(__CONST_FLOAT, v0);

        node_link(f0, n0);

        return f0;
}

struct Node* create_putchar(void)
{
        struct Node* f0 = node_new(__FUNC_PUTCHAR);

        struct Complex* v0 = complex_new('A', 0);
        struct Node* n0 = node_new_leaf(__CONST_FLOAT, v0);

        node_link(f0, n0);

        return f0;
}

struct Node* create_putpixel(void)
{
        struct Node* f0 = node_new(__FUNC_PUTPIXEL);

        struct Complex* v0 = complex_new(100, 0);
        struct Node* n0 = node_new_leaf(__CONST_FLOAT, v0);

        struct Complex* v1 = complex_new(200, 0);
        struct Node* n1 = node_new_leaf(__CONST_FLOAT, v1);

        struct Complex* v2 = complex_new(1, 0);
        struct Node* n2 = node_new_leaf(__CONST_FLOAT, v2);

        node_link(f0, n0);
        node_link(f0, n1);
        node_link(f0, n2);
        node_link(f0, n2);
        node_link(f0, n2);

        return f0;
}

void f(void)
{
        struct Node* n0 = create_putchar();
        struct CalcNode cn0 = calcnode_function(n0);

        struct Node* n1 = create_print();
        struct CalcNode cn1 = calcnode_function(n1);

        struct Node* n2 = create_putpixel();
        struct CalcNode cn2 = calcnode_function(n2);
}

blMain()
{
        mem_init();

        f();

        bl_wait(-1);
}
