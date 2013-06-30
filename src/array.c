/* array.c
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
#include "array.h"

static void** new_array(const size_t array_len,
                        func_array_unit_constructor unit_constructor)
{
        void** a = malloc(sizeof(*a) * array_len);
        if (a == NULL) {
                printf("err: array.c, new_array(), malloc, array_len\n");
                return NULL;
        }

        size_t i;
        for (i = 0; i < array_len; i++) {
                a[i] = unit_constructor();
                if (a[i] == NULL) {
                        printf("err: array.c, new_array(), unit_constructor\n");
                        return NULL;
                }
        }

        return a;
}

static int free_array(void** a,
                      const size_t array_len,
                      func_array_unit_destructor unit_destructor)
{
        size_t i;
        for (i = 0; i < array_len; i++) {
                int err = unit_destructor(a[i]);
                if (err) {
                        printf("err: array.c, free_array(), unit_destructor, a\n");
                        return -1;
                }
        }

        free(a);

        return 0;
}

static void** extend_array(void** old_array,
                           const size_t old_array_len,
                           const size_t new_array_len,
                           func_array_unit_constructor unit_constructor,
                           func_array_unit_destructor unit_destructor,
                           func_array_unit_copy unit_copy)
{
        void** dst = new_array(new_array_len, unit_constructor);
        if (dst == NULL) {
                printf("err: array.c, extend_array(), new_array, dst\n");
                return NULL;
        }

        size_t i;
        for (i = 0; i < old_array_len; i++) {
                int err = unit_copy(dst[i], old_array[i]);
                if (err) {
                        printf("err: array.c, extend_array(), unit_copy\n");
                        return NULL;
                }
        }

        int err = free_array(old_array, old_array_len, unit_destructor);
        if (err) {
                printf("err: array.c, extend_array(), free_array\n");
                return NULL;
        }

        return dst;
}

struct Array* array_new(const size_t array_len,
                        func_array_unit_constructor unit_constructor,
                        func_array_unit_destructor unit_destructor,
                        func_array_unit_copy unit_copy)
{
        struct Array* a = malloc(sizeof(*a));
        if (a == NULL) {
                printf("err: array_new(), malloc, a\n");
                return NULL;
        }

        a->array = new_array(array_len, unit_constructor);
        if (a->array == NULL) {
                printf("err: array_new(), malloc, a->array\n");
                return NULL;
        }

        a->len = array_len;
        a->unit_constructor = unit_constructor;
        a->unit_destructor = unit_destructor;
        a->unit_copy = unit_copy;

        return a;
}

int array_free(struct Array* a)
{
        int err = free_array(a->array, a->len, a->unit_destructor);
        if (err) {
                printf("err: array_free(), a->array\n");
                return -1;
        }

        free(a);

        return 0;
}

int array_extend_x2(struct Array* a)
{
        const size_t old_len = a->len;
        a->len <<= 1;

        a->array = extend_array(a->array, old_len, a->len,
                                a->unit_constructor, a->unit_destructor, a->unit_copy);
        if (a->array == NULL) {
                printf("err: array_extend_x2(), extend_array()\n");
                return -1;
        }

        return 0;
}

int array_write_unit(struct Array* a, const size_t index, void* src)
{
        if (index >= a->len) {
                printf("err: array_write_unit(), write overflow\n");
                return -1;
        }

        int err = a->unit_copy(a->array[index], src);
        if (err) {
                printf("err: array_write_unit()\n");
                return -1;
        }

        return 0;
}

int array_read_unit(struct Array* a, const size_t index, void* dst)
{
        if (index >= a->len) {
                printf("err: array_read_unit(), array read segment error\n");
                return -1;
        }

        int err = a->unit_copy(dst, a->array[index]);
        if (err) {
                printf("err: array_read_unit()\n");
                return -1;
        }

        return 0;
}
