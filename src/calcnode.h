/* calcnode.h
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
#include <stdint.h>
#include "node.h"
#include "complex.h"

#ifndef __CALCNODE_H__
#define __CALCNODE_H__

enum CalcNodeType {
        CNT_NOT_FOUND = 0,
        CNT_BOTTOM,
        CNT_COMPVAL,
        CNT_FUNCPTR,
        CNT_STRPTR,
        CNT_VARPTR,
};

struct CalcNode {
        enum CalcNodeType type;
        struct Complex compval;
        void* ptr;
        struct Complex comparison_val;
        uint64_t comparison_ope;
};

struct CalcNode calcnode(struct Node* a);

#endif /* __CALCNODE_H__ */
