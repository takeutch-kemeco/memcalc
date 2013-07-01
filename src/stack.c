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
#include "array.h"
#include "stack.h"

#define DEFAULT_STACK_LEN 0x100

struct Stack* stack_new(func_array_unit_constructor unit_constructor,
                        func_array_unit_destructor unit_destructor,
                        func_array_unit_copy unit_copy)
{
        struct Stack* a = malloc(sizeof(*a));
        if (a == NULL) {
                printf("err: stack_new(), malloc, a\n");
                return NULL;
        }

        a->head = 0;
        a->array = array_new(DEFAULT_STACK_LEN, unit_constructor, unit_destructor, unit_copy);
        if (a->array == NULL) {
                printf("err: stack_new(), array_new(), a->array\n");
                return NULL;
        }

        return a;
}

int stack_free(struct Stack* a)
{
        int err = array_free(a->array);
        if (err) {
                printf("err: stack_free(), array_free()\n");
                return -1;
        }

        free(a);

        return 0;
}

int stack_push(struct Stack* a, void* unit)
{
        if (a->head >= a->array->len) {
                int err = array_extend_x2(a->array);
                if (err) {
                        printf("err: stack_push(), extend_array(), a->array\n");
                        return -1;
                }
        }

        int err = array_write_unit(a->array, a->head, unit);
        if (err) {
                printf("err: stack_push(), unit_copy\n");
                return -1;
        }

        a->head++;

        return 0;
}

int stack_pop(struct Stack* a, void* unit)
{
        if (a->head == 0) {
                printf("err: stack_pop(), stack_underflow\n");
                return -1;
        }

        a->head--;

        int err = array_read_unit(a->array, a->head, unit);
        if (err) {
                printf("err: stack_pop(), array_read_unit()\n");
                return -1;
        }

        return 0;
}

int stack_read(struct Stack* a, void* unit)
{
        if (a->head == 0) {
                printf("err: stack_read(), stack_underflow\n");
                return -1;
        }

        int err = array_read_unit(a->array, a->head - 1, unit);
        if (err) {
                printf("err: stack_read(), array_read_unit()\n");
                return -1;
        }

        return 0;
}
