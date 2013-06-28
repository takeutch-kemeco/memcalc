/* stack.c
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
#include "stack.h"

static void* new_array(const size_t array_len,
                       func_stack_unit_constructor unit_constructor)
{
        void** a = malloc(sizeof(*a) * array_len);
        if (a == NULL) {
                printf("err: create_array(), malloc, array_len\n");
                return NULL;
        }

        int i;
        for (i = 0; i < array_len; i++) {
                a[i] = unit_constructor();
                if (a[i] == NULL) {
                        printf("err: create_array(), unit_constructor\n");
                        return NULL;
                }
        }

        return a;
}

static int free_array(void** a,
                      const size_t array_len,
                      func_stack_unit_destructor unit_destructor)
{
        int i;
        for (i = 0; i < array_len; i++) {
                int err = unit_destructor(a[i]);
                if (err) {
                        printf("err: delete_array(), unit_destructor, a\n");
                        return -1;
                }
        }

        free(a);

        return 0;
}

static void* extend_array(void** old_array,
                          const size_t old_array_len,
                          const size_t new_array_len,
                          func_stack_unit_constructor unit_constructor,
                          func_stack_unit_destructor unit_destructor,
                          func_stack_unit_copy unit_copy)
{
        void** dst = new_array(new_array_len, unit_constructor);
        if (dst == NULL) {
                printf("err: extend_array(), new_array, dst\n");
                return NULL;
        }

        int i;
        for (i = 0; i < old_array_len; i++) {
                int err = unit_copy(dst[i], old_array[i]);
                if (err) {
                        printf("err: extend_array(), unit_copy\n");
                        return NULL;
                }
        }

        int err = free_array(old_array, old_array_len, unit_destructor);
        if (err) {
                printf("extend_array(), free_array\n");
                return NULL;
        }

        return dst;
}

struct Stack* stack_new(func_stack_unit_constructor unit_constructor,
                        func_stack_unit_destructor unit_destructor,
                        func_stack_unit_copy unit_copy)
{
        struct Stack* a = malloc(sizeof(*a));
        if (a == NULL) {
                printf("err: stack_new(), malloc, a\n");
                return NULL;
        }

        a->len = 0x1;
        a->head = 0;
        a->unit_size;
        a->unit_constructor = unit_constructor;
        a->unit_destructor = unit_destructor;
        a->unit_copy = unit_copy;

        a->array = new_array(a->len, a->unit_constructor);
        if (a->array == NULL) {
                printf("err: stack_new(), new_array, a->array\n");
                return NULL;
        }

        return a;
}

int stack_free(struct Stack* a)
{
        int err = free_array(a->array, a->len, a->unit_destructor);
        if (err) {
                printf("err: stack_free(), delete_array\n");
                return -1;
        }

        free(a);

        return 0;
}

int stack_push(struct Stack* a, void* unit)
{
        if (a->head >= a->len) {
                const size_t old_len = a->len;
                a->len << 1;

                a->array = extend_array(a->array, old_len, a->len,
                                        a->unit_constructor, a->unit_destructor, a->unit_copy);
                if (a->array == NULL) {
                        printf("err: stack_push(), extend_array, a->array\n");
                        return -1;
                }
        }

        int err = a->unit_copy(a->array[a->head], unit);
        if (err) {
                printf("err: stack_push(), unit_copy\n");
                return -1;
        }

        a->head++;

        return 0;
}

void* stack_pop(struct Stack* a)
{
        if (a->head == 0) {
                printf("err: stack_pop(), stack_underflow\n");
                return NULL;
        }

        a->head--;

        return a->array[a->head];
}

void* stack_read(struct Stack* a)
{
        if (a->head == 0) {
                printf("err: stack_read(), stack_underflow\n");
                return NULL;
        }

        return a->array[a->head - 1];
}
