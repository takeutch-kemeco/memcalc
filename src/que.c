/* que.c
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
#include "que.h"

#define DEFAULT_QUE_LEN 0x100

static int inc_head(struct Que* a)
{
        size_t new_head = a->head + 1;

        if (new_head >= a->array->len)
                new_head = 0;

        if (new_head == a->tail)
                return -1;

        a->head = new_head;
        return 0;
}

static int inc_tail(struct Que* a)
{
        if (a->tail == a->head)
                return -1;

        size_t new_tail = a->tail + 1;

        if (new_tail >= a->array->len)
                new_tail = 0;

        a->tail = new_tail;
        return 0;
}

struct Que* que_new(func_array_unit_constructor unit_constructor,
                    func_array_unit_destructor unit_destructor,
                    func_array_unit_copy unit_copy)
{
        struct Que* a = malloc(sizeof(*a));
        if (a == NULL) {
                printf("err: que_new(), malloc, a\n");
                return NULL;
        }

        a->head = 0;
        a->tail = 0;
        a->array = array_new(DEFAULT_QUE_LEN, unit_constructor, unit_destructor, unit_copy);
        if (a->array == NULL) {
                printf("err: que_new(), array_new(), a->array\n");
                return NULL;
        }

        return a;
}

int que_free(struct Que* a)
{
        int err = array_free(a->array);
        if (err) {
                printf("err: que_free(), array_free()\n");
                return -1;
        }

        free(a);

        return 0;
}

int que_enque(struct Que* a, void* unit)
{
        int err = array_write_unit(a->array, a->head, unit);
        if (err) {
                printf("err: que_enque(), array_write_unit()\n");
                return -1;
        }

        if (inc_head(a) != 0) {
                const size_t old_len = a->array->len;
                int err = array_extend_x2(a->array);
                if (err) {
                        printf("err: que_enque(), array_extend_x2(), a->array\n");
                        return -1;
                }

                if (inc_head(a) != 0) {
                        printf("err: que_enque(), inc_head()\n");
                        return -1;
                }
        }

        return 0;
}

int que_deque(struct Que* a, void* unit)
{
        if (a->head == a->tail) {
                printf("err: que_deque(), que empty\n");
                return -1;
        }

        int err = array_read_unit(a->array, a->tail, unit);
        if (err) {
                printf("err: que_deque(), array_read_unit()\n");
                return -1;
        }

        inc_tail(a);

        return 0;
}

int que_read(struct Que* a, void* unit)
{
        if (a->head == a->tail) {
                printf("err: que_read(), que empty\n");
                return -1;
        }

        int err = array_read_unit(a->array, a->tail, unit);
        if (err) {
                printf("err: que_read(), array_read_unit()\n");
                return -1;
        }

        return 0;
}
