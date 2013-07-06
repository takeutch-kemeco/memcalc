/* calcnode_expression.h
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

#include "node.h"
#include "calcnode.h"

#ifndef __CALCNODE_EXPRESSION_H__
#define __CALCNODE_EXPRESSION_H__

struct CalcNode expression__CONST_FLOAT(struct Node* a);
struct CalcNode expression__OPE_REAL_PART(struct Node* a);
struct CalcNode expression__OPE_IMAGINARY_PART(struct Node* a);
struct CalcNode expression__OPE_CONJUGATE(struct Node* a);
struct CalcNode expression__OPE_ARGUMENT(struct Node* a);
struct CalcNode expression__OPE_ABSOLUTE(struct Node* a);
struct CalcNode expression__OPE_COMPLEX(struct Node* a);
struct CalcNode expression__OPE_COMPLEX_POLAR(struct Node* a);
struct CalcNode expression__OPE_ADD(struct Node* a);
struct CalcNode expression__OPE_SUB(struct Node* a);
struct CalcNode expression__OPE_MUL(struct Node* a);
struct CalcNode expression__OPE_DIV(struct Node* a);
struct CalcNode expression__OPE_POWER(struct Node* a);
struct CalcNode expression__OPE_MOD(struct Node* a);
struct CalcNode expression__OPE_LSHIFT(struct Node* a);
struct CalcNode expression__OPE_RSHIFT(struct Node* a);
struct CalcNode expression__OPE_OR(struct Node* a);
struct CalcNode expression__OPE_AND(struct Node* a);
struct CalcNode expression__OPE_XOR(struct Node* a);
struct CalcNode expression__OPE_NOT(struct Node* a);
struct CalcNode expression__OPE_PLUS(struct Node* a);
struct CalcNode expression__OPE_MINUS(struct Node* a);

#endif /* __CALCNODE_EXPRESSION_H__ */
