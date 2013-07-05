/* test_stack.c
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
#include <stdint.h>
#include "stack.h"

static uint64_t t = 123;

static void* constructor(void) {return malloc(sizeof(uint64_t));}
static int destractor(void* a) {free(a); return 0;}

static int copy(void* dst, void* src) {*((uint64_t*)dst) = *((uint64_t*)src); return 0;}

static void f(void)
{
        struct Stack* s = stack_new(constructor, destractor, copy);

        int i = 0;
        for (i = 0; i < 20; i++) {
                stack_push(s, (void*)&t);

                uint64_t tmp;
                stack_read(s, (void*)&tmp);
                printf("stack_push(), stack_read() => %ld\n", tmp);
                t++;
        }

        for (i = 0; i < 20; i++) {
                uint64_t tmp;
                stack_pop(s, (void*)&tmp);
                printf("stack_pop() => %ld\n", tmp);
        }

        stack_free(s);
}

static void g(void)
{
        struct Stack* s = stack_new(constructor, destractor, copy);

        const uint64_t len = 1000000;
        t = len;

        int i = 0;
        for (i = 0; i < len; i++) {
                stack_push(s, (void*)&t);
                t++;
        }

        uint64_t tmp;
        stack_read(s, (void*)&tmp);
        printf("stack_push() * 1M, stack_read() => %ld\n", tmp);

        for (i = 1; i < len; i++) {
                uint64_t tmp;
                stack_pop(s, (void*)&tmp);
        }

        stack_read(s, (void*)&tmp);
        printf("stack_pop() * (1M - 1), stack_read() => %ld\n", tmp);
}

int main(int argc, char** argv)
{
        f();
        f();
        f();

        g();
}
