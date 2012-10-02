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
#include <sys/types.h>
#include "mem.h"

#define POOL_MAX 0x1000
static u_char* pool;

#define REF_NAME_MAX 0x100
struct REF {
        char name[REF_NAME_MAX];
        struct MemTag tag;
};

static struct REF** reflist;
static u_int reflist_head = 0;

void mem_init(void)
{
        pool = malloc(sizeof(*pool) * POOL_MAX);
        memset(pool, 0, POOL_MAX);

        reflist = malloc(sizeof(*reflist) * POOL_MAX);
        memset(reflist, 0, POOL_MAX);

        reflist_head = 0;
}

void mem_close(void)
{
        int i;
        for (i = 0; i < POOL_MAX; i++) {
                if (reflist[i] != NULL)
                        free(reflist[i]);

                reflist[i] = NULL;
        }

        free(reflist);
        free(pool);
}

static struct REF* ref_new(void)
{
        struct REF* a = malloc(sizeof(*a));
        a->name[0] = '\0';
        a->tag.address = 0;
        a->tag.bytesize = 0;

        return a;
}

static void* seek_poolhead(void)
{
        if (reflist_head == 0)
                return &pool[0];

        struct REF* prev = reflist[reflist_head - 1];
        return prev->tag.address + prev->tag.bytesize;
}

void push_var(char* name, size_t bytesize)
{
        if (reflist[reflist_head] == NULL)
                reflist[reflist_head] = ref_new();

        struct REF* p = reflist[reflist_head];
        strncpy(p->name, name, REF_NAME_MAX);
        p->tag.address = seek_poolhead();
        p->tag.bytesize = bytesize;

        reflist_head++;
}

void pop_var(void)
{
        if (reflist_head == 0) {
                printf("error pop_var():\n");
                exit(1);
        }

        reflist_head--;
}

static struct REF* search_ref(char* name)
{
        const size_t name_len = strlen(name);

        int i;
        for (i = reflist_head - 1; i >= 0; i--) {
                struct REF* p = reflist[i];

                if (strlen(p->name) == name_len) {
                        if (strcmp(p->name, name) == 0) {
                                return p;
                        }
                }
        }

        return NULL;
}

struct MemTag* get_ptr_var(char* name)
{
        struct REF* p = search_ref(name);
        if (p == NULL)
                return NULL;

        return &(p->tag);
}
