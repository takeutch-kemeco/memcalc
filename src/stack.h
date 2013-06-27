/* stack.h
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

#ifndef __STACK_H__
#define __STACK_H__

struct Stack {
        size_t len;
        size_t head;
        uint64_t* val;
};

struct Stack* stack_new(void);
void stack_close(struct Stack* a);

void stack_push(struct Stack* a, const uint64_t val);
uint64_t stack_pop(struct Stack* a);

#endif /* __STACK_H__ */
