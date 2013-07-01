/* test_que.c
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
#include "que.h"

static uint64_t t = 123;

static void* constructor(void) {return malloc(sizeof(uint64_t));}
static int destractor(void* a) {free(a); return 0;}

static int copy(void* dst, void* src) {*((uint64_t*)dst) = *((uint64_t*)src); return 0;}

static void f(void)
{
        struct Que* q = que_new(constructor, destractor, copy);

        size_t i;
        for (i = 0; i < 20; i++) {
                que_enque(q, (void*)&t);

                uint64_t tmp;
                que_read(q, (void*)&tmp);
                printf("que_enque(), que_read() => %ld\n", tmp);
                t++;
        }

        for (i = 0; i < 20; i++) {
                uint64_t tmp;
                que_deque(q, (void*)&tmp);
                printf("que_deque() => %ld\n", tmp);
        }

        que_free(q);
}

static void g(void)
{
        struct Que* q = que_new(constructor, destractor, copy);

        const size_t len = 1000000;
        t = len;

        size_t i;
        for (i = 0; i < len; i++) {
                que_enque(q, (void*)&t);
                t++;
        }

        uint64_t tmp;
        que_read(q, (void*)&tmp);
        printf("que_enque() * 1M, que_read() => %ld\n", tmp);

        for (i = 1; i < len; i++) {
                uint64_t tmp;
                que_deque(q, (void*)&tmp);
        }

        que_read(q, (void*)&tmp);
        printf("que_deque() * (1M - 1), que_read() => %ld\n", tmp);

        que_free(q);
}

int main(int argc, char** argv)
{
        f();
        f();
        f();

        g();
}
