/* test_array.c
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
#include "array.h"

static uint64_t t = 123;

static void* constructor(void) {return malloc(sizeof(uint64_t));}
static int destractor(void* a) {free(a); return 0;}

static int copy(void* dst, void* src) {*((uint64_t*)dst) = *((uint64_t*)src); return 0;}

static void f(void)
{
        struct Array* a = array_new(20, constructor, destractor, copy);

        int i = 0;
        for (i = 0; i < 20; i++) {
                array_write_unit(a, i, (void*)&t);

                uint64_t tmp;
                array_read_unit(a, i, (void*)&tmp);

                printf("array_write_unit(), array_read_unit() => %ld\n", tmp);
                t++;
        }

        array_free(a);
}

static void g(void)
{
        const size_t len = 1000000;
        t = len;

        struct Array* a = array_new(t, constructor, destractor, copy);

        int i = 0;
        for (i = 0; i < len; i++) {
                array_write_unit(a, i, (void*)&t);
                t++;
        }

        uint64_t tmp;
        array_read_unit(a, 0, (void*)&tmp);
        printf("array_write_unit() * 1M, array_read_read()[0] => %ld\n", tmp);

        array_read_unit(a, len-1, (void*)&tmp);
        printf("array_write_unit() * 1M, array_read_read()[%d] => %ld\n", len-1, tmp);

        array_free(a);
}

int main(int argc, char** argv)
{
        f();
        f();
        f();

        g();
}
