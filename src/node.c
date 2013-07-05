/* node.c
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
#include "node.h"

#define DEFAULT_NODE_LEN 2

static void* voidptr_constructor(void)
{
        void** a = malloc(sizeof(*a));
        if (a == NULL) {
                printf("err: node.c, voidptr_constructor(), malloc, a\n");
                return NULL;
        }

        return a;
}

static int voidptr_destructor(void* a)
{
        free(a);
        return 0;
}

static int voidptr_copy(void* dst, void* src)
{
        *((void**)dst) = *((void**)src);
        return 0;
}

struct Node* node_new(const uint64_t ope)
{
        struct Node* a = malloc(sizeof(*a));
        if (a == NULL) {
                printf("err: node_new(), malloc, a\n");
                return NULL;
        }

        a->child = array_new(DEFAULT_NODE_LEN, voidptr_constructor, voidptr_destructor, voidptr_copy);
        if (a->child == NULL) {
                printf("err: node_new(), array_new(), a->child\n");
                return NULL;
        }

        a->child_len = 0;
        a->ope = ope;

        return a;
}

struct Node* node_new_leaf(const uint64_t ope, void* val)
{
        struct Node* a = node_new(ope);
        if (a == NULL) {
                printf("err: node_new_leaf(), node_new(), a\n");
                return NULL;
        }

        int err = node_link(a, (struct Node*)val);
        if (err) {
                printf("err: node_new_leaf(), node_link()\n");
                return NULL;
        }

        return a;
}

int node_free(struct Node* a)
{
        int err = array_free(a->child);
        if (err) {
                printf("err: node_free(), array_free()\n");
                return -1;
        }

        free(a);

        return 0;
}

int node_link(struct Node* super, struct Node* child)
{
        if (super->child_len >= super->child->len) {
                int err = array_extend_x2(super->child);
                if (err) {
                        printf("err: node_link(), extend_array(), super->child\n");
                        return -1;
                }
        }

        int err = array_write_unit(super->child, super->child_len, &child);
        if (err) {
                printf("err: node_link(), array_write_unit()\n");
                return -1;
        }

        super->child_len++;

        return 0;
}

struct Node* node_child(struct Node* a, const size_t index)
{
        if (a->child_len == 0) {
                printf("err: node_child(), child undefined\n");
                return NULL;
        }

        struct Node* child;
        int err = array_read_unit(a->child, index, &child);
        if (err) {
                printf("err: node_read(), array_read_unit()\n");
                return NULL;
        }

        return child;
}
