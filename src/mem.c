/* mem.c
 * Copyright (C) 2012 Takeutch Kemeco
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
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include "mem.h"

#define POOL_MAX 0x1000000
static u_char* pool;

struct REF {
        char* name;
        struct MemTag tag;
};

static struct REF* reflist;
static u_int reflist_head = 0;

void mem_init(void)
{
        pool = malloc(sizeof(*pool) * POOL_MAX);
        reflist = calloc(sizeof(*reflist), POOL_MAX);   /* 0 で初期化するため */

        reflist_head = 0;
}

static void* clear_ref(struct REF* a)
{
        if (a->name != NULL) {
                free(a->name);
                a->name = NULL;
        }

        a->tag.address = 0;
        a->tag.bytesize = 0;
        a->tag.index = 0;
}

void mem_close(void)
{
        int i;
        for (i = 0; i < POOL_MAX; i++)
                clear_ref(reflist + i);

        free(reflist);
        free(pool);
}

static void* seek_poolhead(void)
{
        if (reflist_head == 0)
                return &pool[0];

        struct REF* p = &reflist[reflist_head - 1];
        return p->tag.address + p->tag.bytesize;
}

static void push_var(const char* name, const size_t bytesize)
{
        struct REF* p = &reflist[reflist_head];

        clear_ref(p);

        p->name = malloc(strlen(name) + 1);
        strcpy(p->name, name);

        p->tag.address = seek_poolhead();
        p->tag.bytesize = bytesize;

        reflist_head++;
}

static void pop_var(void)
{
        if (reflist_head == 0) {
                printf("system error: pop_var()\n");
                return;
        }

        reflist_head--;
}

static struct REF* search_ref(const char* name)
{
        const size_t name_len = strlen(name);

        int i;
        for (i = reflist_head - 1; i >= 0; i--) {
                struct REF* p = &reflist[i];

                if (strlen(p->name) == name_len) {
                        if (strcmp(p->name, name) == 0) {
                                return p;
                        }
                }
        }

        return NULL;
}

static struct MemTag* get_ptr_var(const char* name)
{
        struct REF* p = search_ref(name);
        if (p == NULL)
                return NULL;

        return &(p->tag);
}

struct MemTag* read_num_var_memtag(const char* name, const size_t index)
{
        struct MemTag* p = get_ptr_var(name);

        if (p == NULL) {
                push_var(name, sizeof(double) * index);
                p = get_ptr_var(name);
        }

        p->index = index;
        return p;
}

double read_num_var_value(const char* name, const size_t index)
{
        struct MemTag* p = read_num_var_memtag(name, index);

        double* tmp = (double*)(p->address);
        return tmp[p->index];
}
