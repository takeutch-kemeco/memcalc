/* test_node.c
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
#include "node.h"

static uint64_t un = 0;

static void create_child(struct Node* s, int len)
{
        struct Node* c[100];
        int i;
        for (i = 0; i < len; i++) {
                c[i] = node_new(un++);
                printf("[%p]:[%p]:[%d]\n", s, c[i], c[i]->ope);
        }
        printf("\n");

        for (i = 0; i < len; i++) {
                node_link(s, c[i]);
        }

        for (i = 0; i < len; i++) {
                struct Node* tmp = node_child(s, i);
                printf("[%p]:[%p]:[%d]\n", s, c[i], c[i]->ope);
        }
        printf("[%p]:[%p]:[%d]\n", s, c[0], c[0]->ope);
        printf("\n");
}

void f(void)
{
        struct Node* s = node_new(1);
        create_child(s, 5);

        int i;
        for (i = 0; i < 5; i++) {
                struct Node* tmp = node_child(s, i);
                create_child(tmp, 3);
        }
}

int main(int argc, char** argv)
{
        f();
}
