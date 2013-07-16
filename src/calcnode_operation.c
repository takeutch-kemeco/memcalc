/* calcnode_operation.c
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
#include "memcalc.bison.h"

static struct CalcNode operation__CONST_FLOAT(struct Node* a)
{
        struct Complex* tmp = (struct Complex*)node_child(a, 0);
        struct CalcNode cn0 = {.type = CNT_COMPVAL, .compval = *tmp};

        return cn0;
}

static struct CalcNode operation__OPE_REAL_PART(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        cn0.compval = complex_realpart_safe(cn0.compval);

        return cn0;
}

static struct CalcNode operation__OPE_IMAGINARY_PART(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        cn0.compval = complex_imagpart_safe(cn0.compval);

        return cn0;
}

static struct CalcNode operation__OPE_CONJUGATE(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        cn0.compval = complex_conjugate(cn0.compval);

        return cn0;
}

static struct CalcNode operation__OPE_ARGUMENT(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        cn0.compval = complex_constructor(complex_imagpart(complex_polar(cn0.compval)), 0);

        return cn0;
}

static struct CalcNode operation__OPE_ABSOLUTE(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        cn0.compval = complex_abs(cn0.compval);

        return cn0;
}

static struct CalcNode operation__OPE_COMPLEX(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));

        cn0.compval = complex_constructor(complex_realpart(cn0.compval), complex_realpart(cn1.compval));

        return cn0;
}

static struct CalcNode operation__OPE_COMPLEX_POLAR(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));

        cn0.compval = complex_mk_polar(complex_constructor(complex_realpart(cn0.compval), complex_realpart(cn1.compval)));

        return cn0;
}

static struct CalcNode operation__OPE_ADD(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));

        cn0.compval = complex_add(cn0.compval, cn1.compval);

        return cn0;
}

static struct CalcNode operation__OPE_SUB(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));

        cn0.compval = complex_sub(cn0.compval, cn1.compval);

        return cn0;
}

static struct CalcNode operation__OPE_MUL(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));

        cn0.compval = complex_mul(cn0.compval, cn1.compval);

        return cn0;
}

static struct CalcNode operation__OPE_DIV(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));

        cn0.compval = complex_div(cn0.compval, cn1.compval);

        return cn0;
}

static struct CalcNode operation__OPE_POWER(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));

        cn0.compval = complex_pow(cn0.compval, cn1.compval);

        return cn0;
}

static struct CalcNode operation__OPE_MOD(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));

        cn0.compval = complex_mod(cn0.compval, cn1.compval);

        return cn0;
}

static struct CalcNode operation__OPE_LSHIFT(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));

        cn0.compval = complex_lshift(cn0.compval, cn1.compval);

        return cn0;
}

static struct CalcNode operation__OPE_RSHIFT(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));

        cn0.compval = complex_rshift(cn0.compval, cn1.compval);

        return cn0;
}

static struct CalcNode operation__OPE_OR(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));

        cn0.compval = complex_or(cn0.compval, cn1.compval);

        return cn0;
}

static struct CalcNode operation__OPE_AND(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));

        cn0.compval = complex_and(cn0.compval, cn1.compval);

        return cn0;
}

static struct CalcNode operation__OPE_XOR(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));

        cn0.compval = complex_xor(cn0.compval, cn1.compval);

        return cn0;
}

static struct CalcNode operation__OPE_NOT(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        cn0.compval = complex_not(cn0.compval);

        return cn0;
}

static struct CalcNode operation__OPE_PLUS(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        return cn0;
}

static struct CalcNode operation__OPE_MINUS(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        cn0.compval = complex_constructor(-complex_realpart(cn0.compval), -complex_imagpart(cn0.compval));

        return cn0;
}

struct CalcNode calcnode_operation(struct Node* a)
{
        switch (a->ope) {
        case __CONST_FLOAT:             return operation__CONST_FLOAT(a);
        case __OPE_REAL_PART:           return operation__OPE_REAL_PART(a);
        case __OPE_IMAGINARY_PART:      return operation__OPE_IMAGINARY_PART(a);
        case __OPE_CONJUGATE:           return operation__OPE_CONJUGATE(a);
        case __OPE_ARGUMENT:            return operation__OPE_ARGUMENT(a);
        case __OPE_ABSOLUTE:            return operation__OPE_ABSOLUTE(a);
        case __OPE_COMPLEX:             return operation__OPE_COMPLEX(a);
        case __OPE_COMPLEX_POLAR:       return operation__OPE_COMPLEX_POLAR(a);
        case __OPE_ADD:                 return operation__OPE_ADD(a);
        case __OPE_SUB:                 return operation__OPE_SUB(a);
        case __OPE_MUL:                 return operation__OPE_MUL(a);
        case __OPE_DIV:                 return operation__OPE_DIV(a);
        case __OPE_POWER:               return operation__OPE_POWER(a);
        case __OPE_MOD:                 return operation__OPE_MOD(a);
        case __OPE_LSHIFT:              return operation__OPE_LSHIFT(a);
        case __OPE_RSHIFT:              return operation__OPE_RSHIFT(a);
        case __OPE_OR:                  return operation__OPE_OR(a);
        case __OPE_AND:                 return operation__OPE_AND(a);
        case __OPE_XOR:                 return operation__OPE_XOR(a);
        case __OPE_NOT:                 return operation__OPE_NOT(a);
        case __OPE_PLUS:                return operation__OPE_PLUS(a);
        case __OPE_MINUS:               return operation__OPE_MINUS(a);
        }

        struct CalcNode cn = {.type = CNT_NOT_FOUND};
        return cn;
}
