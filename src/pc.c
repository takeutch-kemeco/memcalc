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

#include <stdlib.h>
#include "stack.h"

static struct Stack* pc_stack;

static void* pc_unit_constructor(void)
{
        return malloc(sizeof(uint64_t));
}

static int pc_unit_destructor(void* a)
{
        free(a);
        return 0;
}

static int pc_unit_copy(void* dst, void* src)
{
        *((uint64_t*)dst) = *((uint64_t*)src);
        return 0;
}

void pc_init(void)
{
        pc_stack = stack_new(pc_unit_constructor, pc_unit_destructor, pc_unit_copy);
}

void pc_close(void)
{
        stack_free(pc_stack);
}

void pc_push(const uint64_t fpos)
{
        stack_push(pc_stack, (void*)&fpos);
}

uint64_t pc_pop(void)
{
        uint64_t tmp;
        stack_pop(pc_stack, (void*)&tmp);
        return tmp;
}
