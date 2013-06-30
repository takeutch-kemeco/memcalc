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
        struct Que* s = que_new(constructor, destractor, copy);

        int i = 0;
        for (i = 0; i < 20; i++) {
                que_enque(s, (void*)&t);
                printf("que_enque(), que_read() => %ld\n", *((uint64_t*)que_read(s)));
                t++;
        }

        for (i = 0; i < 20; i++) {
                printf("que_deque() => %ld\n", *((uint64_t*)que_deque(s)));
        }

        que_free(s);
}

static void g(void)
{
        struct Que* s = que_new(constructor, destractor, copy);

        t = 1000000;

        int i = 0;
        for (i = 0; i < 1000000; i++) {
                que_enque(s, (void*)&t);
                t++;
        }

        printf("que_enque() * 1M, que_read() => %ld\n", *((uint64_t*)que_read(s)));

        for (i = 1; i < 1000000; i++) {
                que_deque(s);
        }

        printf("que_deque() * (1M - 1), que_read() => %ld\n", *((uint64_t*)que_read(s)));
}

int main(int argc, char** argv)
{
        f();
        f();
        f();

        g();
}
