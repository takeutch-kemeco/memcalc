/* pc.c
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

#include "stack.h"

static struct Stack* pc_stack;

void pc_init(void)
{
        pc_stack = stack_new();
}

void pc_close(void)
{
        stack_close(pc_stack);
}

void pc_push(const uint64_t fpos)
{
        stack_push(pc_stack, fpos);
}

uint64_t pc_pop(void)
{
        return stack_pop(pc_stack);
}
