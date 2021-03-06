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

static int indent = 0;
static uint64_t un = 0;

static void create_child(struct Node* s, const int len)
{
        int i;
        for (i = 0; i < len; i++, un++) {
                struct Node* tmp = node_new(un);
                create_child(tmp, len - 1);

                node_link(s, tmp);
        }
}

struct Node* f(const int len)
{
        struct Node* s = node_new(12345);
        create_child(s, len);

        return s;
}

static void print_node(struct Node* s)
{
        indent++;

        int i;
        for (i = 1; i < indent; i++)
                printf("\t");

        printf("s[%p, %d], s.ChildLen:[%d]\n", s, s->ope, s->child_len);

        for (i = 0; i < s->child_len; i++)
                print_node(node_child(s, i));

        indent--;
}

int main(int argc, char** argv)
{
        print_node(f(4));
}
