/* test_calcnode_comparison.c
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
#include "calcnode_comparison.h"
#include "memcalc.bison.h"

static struct Node* create__CONST_FLOAT(const float a)
{
        struct Complex* n0val = complex_new(a, 0);
        struct Node* n0 = node_new_leaf(__CONST_FLOAT, (void*)n0val);

        return n0;
}

static struct Node* create_comparison_unit(const uint64_t ope, const float a)
{
        struct Node* f0 = node_new(ope);
        struct Node* n0 = create__CONST_FLOAT(a);

        node_link(f0, n0);

        return f0;
}

/*
 * comparison +- expression
 *            |
 *            comparison_unit
 *
 * f0 +- (n0 - a)
 *    |
 *    (n1 - b)
 */
static struct Node* create_comparison_A(const float a, const uint64_t ope, const float b)
{
        struct Node* f0 = node_new(__COMPARISON);

        struct Node* n0 = create__CONST_FLOAT(a);

        struct Node* n1 = create_comparison_unit(ope, b);

        node_link(f0, n0);
        node_link(f0, n1);

        return f0;
}

static void print_calcnode(struct CalcNode cn0)
{
        printf("CNT:[%d], compval:[%f, %f], comparison_val[%f, %f]\n\n",
               cn0.type,
               cn0.compval.re, cn0.compval.im,
               cn0.comparison_val.re, cn0.comparison_val.im);
}

static void f(void)
{
        void ff(const float a, const uint64_t ope, const float b)
        {
                struct Node* n0 = create_comparison_A(a, ope, b);
                struct CalcNode cn0 = calcnode(n0);

                switch (ope) {
                case __OPE_COMPARISON:
                        printf("%f == %f -> ", a, b);
                        break;

                case __OPE_NOT_COMPARISON:
                        printf("%f != %f -> ", a, b);
                        break;
                }

                print_calcnode(cn0);
        }

        ff(10, __OPE_COMPARISON, 10);
        ff(11, __OPE_COMPARISON, 10);

        ff(10, __OPE_NOT_COMPARISON, 10);
        ff(11, __OPE_NOT_COMPARISON, 10);
}

/*
 * comparison +- expression
 *            |
 *            comparison_unit_list +- comparison_unit
 *                                 |
 *                                 comparison_unit_list +- comparison_unit
 *                                                      |
 *                                                      comparison_unit
 *
 * f0 +- (n0 - a)
 *    |
 *    u0 +- (f1 - b)
 *       |
 *       u1 +- (f2 - c)
 *          |
 *          (f3 - d)
 */
static struct Node* create_comparison_B(const float a,
                                        const uint64_t ope_b, const float b,
                                        const uint64_t ope_c, const float c,
                                        const uint64_t ope_d, const float d)
{
        struct Node* f0 = node_new(__COMPARISON);

        struct Node* n0 = create__CONST_FLOAT(a);
        struct Node* f1 = create_comparison_unit(ope_b, b);
        struct Node* f2 = create_comparison_unit(ope_c, c);
        struct Node* f3 = create_comparison_unit(ope_d, d);

        struct Node* u0 = node_new(__COMPARISON_UNIT_LIST);
        struct Node* u1 = node_new(__COMPARISON_UNIT_LIST);

        node_link(f0, n0);
        node_link(f0, u0);

        node_link(u0, f1);
        node_link(u0, u1);

        node_link(u1, f2);
        node_link(u1, f3);

        return f0;
}

static struct CalcNode x_calcnode(struct Node* a)
{
        return calcnode(a);
}

static void h(void)
{
        void hh(const float a, const float b, const float c, const float d)
        {
                struct Node* n0 = create_comparison_B(a,
                                                      __OPE_COMPARISON, b,
                                                      __OPE_NOT_COMPARISON, c,
                                                      __OPE_ISSMALL, d);

                struct CalcNode cn0 = x_calcnode(n0);

                printf("%f == %f != %f < %f -> ", a, b, c, d);
                print_calcnode(cn0);
        }

        hh(10, 10, 11, 12);
        hh(20, 10, 11, 12);
        hh(10, 20, 11, 12);
        hh(10, 10, 20, 12);
        hh(10, 10, 11, 10);
}

blMain()
{
        mem_init();

        f();
        h();

        mem_close();
}
