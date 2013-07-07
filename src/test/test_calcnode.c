/* test_calcnode.c
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
#include <string.h>
#include <stdint.h>
#include "complex.h"
#include "mem.h"
#include "node.h"
#include "calcnode.h"
#include "calcnode_function.h"
#include "memcalc.bison.h"

struct Node* create_declarator_scalar(void)
{
        struct Node* f0 = node_new(__DECLARATOR);

        const char* str = "aiueo";
        char* iden = malloc(sizeof(*iden));
        strcpy(iden, str);
        struct Node* n0 = node_new_leaf(__IDENTIFIER, (void*)iden);

        node_link(f0, n0);

        return f0;
}

struct Node* create_declarator_array(void)
{
        struct Node* f0 = node_new(__DECLARATOR);

        const char* str = "kakikukeko";
        char* iden = malloc(sizeof(*iden));
        strcpy(iden, str);
        struct Node* n0 = node_new_leaf(__IDENTIFIER, (void*)iden);

        struct Complex* index = complex_new(10, 0);
        struct Node* n1 = node_new_leaf(__CONST_FLOAT, (void*)index);

        node_link(f0, n0);
        node_link(f0, n1);

        return f0;
}

void f(void)
{
        const char* m0name = "aiueo";
        mem_create_var(m0name, 100);
        struct MemTag* m0 = mem_read_var_memtag(m0name, 0);
        m0->address = (void*)complex_new(123, 456);
        printf("f(), &m0:[%p] ", m0);
        struct Complex* cn0vp = (struct Complex*)m0->address;
        printf("m0->address[%f, %f]\n", complex_realpart(*cn0vp), complex_imagpart(*cn0vp));

        const char* m1name = "kakikukeko";
        mem_create_var(m1name, 100);
        struct MemTag* m1 = mem_read_var_memtag(m1name, 0);
        m1->address = (void*)complex_new(1234, 5678);
        printf("f(), &m1:[%p] ", m1);
        struct Complex* cn1vp = (struct Complex*)m1->address;
        printf("m1->address[%f, %f]\n", complex_realpart(*cn1vp), complex_imagpart(*cn1vp));
}

void g(void)
{
        struct Node* n0 = create_declarator_scalar();
        struct CalcNode cn0 = calcnode(n0);
        struct MemTag* m0 = cn0.ptr;
        printf("g(), &m0:[%p] ", m0);
        struct Complex* cn0vp = (struct Complex*)(m0->address);
        printf("n0[%f, %f]\n", complex_realpart(*cn0vp), complex_imagpart(*cn0vp));

        struct Node* n1 = create_declarator_array();
        struct CalcNode cn1 = calcnode(n1);
        struct MemTag* m1 = cn1.ptr;
        printf("g(), &m0:[%p] ", m1);
        struct Complex* cn1vp = (struct Complex*)(m1->address);
        printf("n1[%f, %f]\n", complex_realpart(*cn1vp), complex_imagpart(*cn1vp));
}

blMain()
{
        mem_init();

        f();
        g();

        bl_wait(-1);
        mem_close();
}
