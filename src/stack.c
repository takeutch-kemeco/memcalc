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

struct Stack* stack_new(void)
{
        struct Stack* a = malloc(sizeof(*a));
        if (a == NULL) {
                printf("err: stack_new(), a\n");
                exit(1);
        }

        a->len = 0x1000;
        a->head = 0;
        a->val = malloc(sizeof(*a->val) * a->len);
        if (a->val == NULL) {
                printf("err: stack_new(), a->val\n");
                exit(1);
        }

        return a;
}

void stack_close(struct Stack* a)
{
        free(a->val);
        free(a);
}

void stack_push(struct Stack* a, const uint64_t val)
{
        if (a->head >= a->len) {
                a->len <<= 1;
                a->val = realloc(a->val, a->len);
                if (a->val == NULL) {
                        printf("err: stack_push(), realloc\n");
                        exit(1);
                }
        }

        a->val[a->head] = val;
        a->head++;
}

uint64_t stack_pop(struct Stack* a)
{
        if (a->head == 0) {
                printf("err: stack_pop(), stack_underflow\n");
                exit(1);
        }

        a->head--;
        return a->val[a->head];
}
