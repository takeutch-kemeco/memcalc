/* que.h
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

#ifndef __QUE_H__
#define __QUE_H__

typedef void* (*func_que_unit_constructor)(void);
typedef int (*func_que_unit_destructor)(void* a);
typedef int (*func_que_unit_copy)(void* dst, void* src);

struct Que {
        size_t len;
        size_t head;
        size_t tail;
        void** array;
        func_que_unit_constructor unit_constructor;
        func_que_unit_destructor unit_destructor;
        func_que_unit_copy unit_copy;
};

struct Que* que_new(func_que_unit_constructor unit_constructor,
                    func_que_unit_destructor unit_destructor,
                    func_que_unit_copy unit_copy);
int que_free(struct Que* a);

int que_enque(struct Que* a, void* unit);
void* que_deque(struct Que* a);

void* que_read(struct Que* a);

#endif /* __QUE_H__ */
