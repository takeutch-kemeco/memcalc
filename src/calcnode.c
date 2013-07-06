/* calcnode.c
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
#include "calcnode.h"
#include "calcnode_expression.h"
#include "memcalc.bison.h"

struct CalcNode calcnode_expression(struct Node* a)
{
        switch (a->ope) {
        case __CONST_FLOAT:             return expression__CONST_FLOAT(a);
        case __OPE_REAL_PART:           return expression__OPE_REAL_PART(a);
        case __OPE_IMAGINARY_PART:      return expression__OPE_IMAGINARY_PART(a);
        case __OPE_CONJUGATE:           return expression__OPE_CONJUGATE(a);
        case __OPE_ARGUMENT:            return expression__OPE_ARGUMENT(a);
        case __OPE_ABSOLUTE:            return expression__OPE_ABSOLUTE(a);
        case __OPE_COMPLEX:             return expression__OPE_COMPLEX(a);
        case __OPE_COMPLEX_POLAR:       return expression__OPE_COMPLEX_POLAR(a);
        case __OPE_ADD:                 return expression__OPE_ADD(a);
        case __OPE_SUB:                 return expression__OPE_SUB(a);
        case __OPE_MUL:                 return expression__OPE_MUL(a);
        case __OPE_DIV:                 return expression__OPE_DIV(a);
        case __OPE_POWER:               return expression__OPE_POWER(a);
        case __OPE_MOD:                 return expression__OPE_MOD(a);
        case __OPE_LSHIFT:              return expression__OPE_LSHIFT(a);
        case __OPE_RSHIFT:              return expression__OPE_RSHIFT(a);
        case __OPE_OR:                  return expression__OPE_OR(a);
        case __OPE_AND:                 return expression__OPE_AND(a);
        case __OPE_XOR:                 return expression__OPE_XOR(a);
        case __OPE_NOT:                 return expression__OPE_NOT(a);
        case __OPE_PLUS:                return expression__OPE_PLUS(a);
        case __OPE_MINUS:               return expression__OPE_MINUS(a);
        }

        struct CalcNode cn = {.type = CNT_EMPTY};
        return cn;
}
