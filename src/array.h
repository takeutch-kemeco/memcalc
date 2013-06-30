/* array.h
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

#ifndef __ARRAY_H__
#define __ARRAY_H__

typedef void* (*func_array_unit_constructor)(void);
typedef int (*func_array_unit_destructor)(void* a);
typedef int (*func_array_unit_copy)(void* dst, void* src);

struct Array {
        size_t len;
        void** array;
        func_array_unit_constructor unit_constructor;
        func_array_unit_destructor unit_destructor;
        func_array_unit_copy unit_copy;
};

struct Array* array_new(const size_t array_len,
                        func_array_unit_constructor unit_constructor,
                        func_array_unit_destructor unit_destructor,
                        func_array_unit_copy unit_copy);
int array_free(struct Array* a);

int array_extend_x2(struct Array* a);

int array_write_unit(struct Array* a, const size_t index, void* src);
int array_read_unit(struct Array* a, const size_t index, void* dst);

#endif /* __ARRAY_H__ */
