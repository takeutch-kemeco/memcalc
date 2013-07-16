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
        mem_create_var(m0name, MTT_COMPVAL, 100);
        struct MemTag* m0 = mem_read_var_memtag(m0name, 0);
        m0->address = (void*)complex_new(123, 456);
        printf("f(), &m0:[%p] ", m0);
        struct Complex* cn0vp = (struct Complex*)m0->address;
        printf("m0->address[%f, %f]\n", complex_realpart(*cn0vp), complex_imagpart(*cn0vp));

        const char* m1name = "kakikukeko";
        mem_create_var(m1name, MTT_COMPVAL, 100);
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

void test_declarator(void)
{
        printf("\ntest_declarator\n");
        f();
        g();
}

void test_assignment(void)
{
        printf("\ntest_assignment\n");

        {
                struct Node* f0 = node_new(__ASSIGNMENT);

                struct Node* n0 = create_declarator_scalar();

                struct Complex* n1val = complex_new(111,222);
                struct Node* n1 = node_new_leaf(__CONST_FLOAT, (void*)n1val);

                node_link(f0, n0);
                node_link(f0, n1);

                calcnode(f0);
        }

        {
                struct Node* f0 = node_new(__ASSIGNMENT);

                struct Node* n0 = create_declarator_array();

                struct Complex* n1val = complex_new(333,444);
                struct Node* n1 = node_new_leaf(__CONST_FLOAT, (void*)n1val);

                node_link(f0, n0);
                node_link(f0, n1);

                calcnode(f0);
        }

        g();
}

struct Node* create_selection_if(void)
{
        struct Node* f0 = node_new(__SELECTION_IF);

        struct Complex* n0val = complex_new(1, 0);
        struct Node* n0 = node_new_leaf(__CONST_FLOAT, (void*)n0val);

        struct Complex* n1val = complex_new(2, 0);
        struct Node* n1 = node_new_leaf(__CONST_FLOAT, (void*)n1val);

        struct Complex* n2val = complex_new(3, 0);
        struct Node* n2 = node_new_leaf(__CONST_FLOAT, (void*)n2val);

        node_link(f0, n0);
        node_link(f0, n1);
        node_link(f0, n2);

        return f0;
}

struct Node* create_selection_exp(void)
{
        struct Node* n0 = create_selection_if();
        n0->ope = __SELECTION_EXP;
        return n0;
}

void test_selection_if(void)
{
        printf("\ntest_selection_if\n");

        struct Node* n0 = create_selection_if();
        struct CalcNode cn0 = calcnode(n0);

        switch (cn0.type) {
        case CNT_FUNCPTR:
                {
                        struct CalcNode cn1 = calcnode((struct Node*)(cn0.ptr));
                        printf("CNT_FUNCPTR, val:[%f, %f]\n", cn1.compval.re, cn1.compval.im);
                }
                break;

        case CNT_BOTTOM:
                printf("CNT_BOTTOM\n");

        default:
                printf("err: CNT_?\n");
                break;
        }
}

void test_selection_exp(void)
{
        printf("\ntest_selection_exp\n");

        struct Node* n0 = create_selection_exp();
        struct CalcNode cn0 = calcnode(n0);

        printf("cn0.compval:[%f, %f]\n", cn0.compval.re, cn0.compval.im);
}

blMain()
{
        mem_init();

        test_declarator();
        test_assignment();
        test_selection_if();
        test_selection_exp();

        mem_close();
}
