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
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "mem.h"
#include "complex.h"
#include "stack.h"

#define POOL_MAX 0x1000000
static void* pool;

struct REF {
        char* name;
        struct MemTag tag;
};

static struct REF* reflist;
static uint64_t reflist_head = 0;

static struct Stack* mem_stack;

static void* mem_stack_unit_constructor(void)
{
        void* p = malloc(sizeof(uint64_t));
        if (p == NULL) {
                printf("mem.c, mem_stack_unit_constructor(), malloc()\n");
                exit(1);
        }

        return p;
}

static int mem_stack_unit_destructor(void* a)
{
        free(a);
        return 0;
}

static int mem_stack_unit_copy(void* dst, void* src)
{
        *((uint64_t*)dst) = *((uint64_t*)src);
        return 0;
}

void mem_init(void)
{
        pool = malloc(sizeof(*pool) * POOL_MAX);
        if (pool == NULL) {
                printf("err: mem_init(), malloc(), pool\n");
                exit(1);
        }

        reflist = calloc(sizeof(*reflist), POOL_MAX);   /* 0 で初期化するため */
        if (calloc == NULL) {
                printf("err: mem_init(), calloc(), reflist\n");
                exit(1);
        }

        reflist_head = 0;

        mem_stack = stack_new(mem_stack_unit_constructor,
                              mem_stack_unit_destructor,
                              mem_stack_unit_copy);

        stack_push(mem_stack, (void*)&reflist_head);
}

static void* clear_ref(struct REF* a)
{
        if (a->name != NULL) {
                free(a->name);
                a->name = NULL;
        }

        a->tag.type = MTT_NOT_FOUND;
        a->tag.address = 0;
        a->tag.type_bytesize = 0;
        a->tag.array_bytesize = 0;
        a->tag.index = 0;
}

void mem_close(void)
{
        int i;
        for (i = 0; i < POOL_MAX; i++)
                clear_ref(reflist + i);

        free(reflist);
        free(pool);

        stack_free(mem_stack);
}

static struct REF* seek_reflist_next_head(void)
{
        return reflist + reflist_head;
}

static struct REF* seek_reflist_cur_head(void)
{
        if (reflist_head != 0)
                return reflist + reflist_head - 1;

        return NULL;
}

static void* seek_poolhead(void)
{
        struct REF* p = seek_reflist_cur_head();
        if (p == NULL)
                return pool;

        return p->tag.address + p->tag.array_bytesize;
}

static void push_var(const char* name,
                     const enum MemTagType type,
                     const size_t type_bytesize,
                     const size_t array_bytesize)
{
        struct REF* p = seek_reflist_next_head();

        clear_ref(p);

        p->name = malloc(strlen(name) + 1);
        strcpy(p->name, name);

        p->tag.type = type;
        p->tag.address = seek_poolhead();
        p->tag.type_bytesize = type_bytesize;
        p->tag.array_bytesize = array_bytesize;
        p->tag.index = 0;

        reflist_head++;
}

static struct REF* search_ref(const char* name)
{
        const size_t name_len = strlen(name);

        struct REF* p = seek_reflist_cur_head();
        if (p == NULL)
                return NULL;

        int i;
        for (i = 0; i < reflist_head; i++) {
                if (strlen(p->name) == name_len)
                        if (strcmp(p->name, name) == 0)
                                return p;

                p--;
        }

        return NULL;
}

static struct REF* search_overlide_ref(const char* name)
{
        const size_t name_len = strlen(name);

        struct REF* p = seek_reflist_cur_head();

        uint64_t reflist_overlide_botom;
        int err = stack_read(mem_stack, (void*)&reflist_overlide_botom);
        if (err) {
                printf("err: search_overlide_ref(), stack_read()\n");
                exit(1);
        }

        int i;
        for (i = reflist_overlide_botom; i < reflist_head; i++) {
                if (strlen(p->name) == name_len)
                        if (strcmp(p->name, name) == 0)
                                return p;

                p--;
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

bool mem_create_var(const char* name,
                    const enum MemTagType type,
                    size_t array_len)
{
        if (search_overlide_ref(name) != NULL)
                return false;

        if (array_len == 0)
                array_len = 1;

        array_len += 1;

        size_t type_bytesize;

        switch (type) {
        case MTT_NOT_FOUND:
                printf("err: mem_create_var(), type\n");
                exit(1);

        case MTT_COMPVAL:
                type_bytesize = sizeof(struct Complex);
                break;

        case MTT_FUNCPTR:
        case MTT_VARPTR:
                type_bytesize = sizeof(void*);
                break;

        }

        const size_t array_bytesize = type_bytesize * array_len;

        push_var(name, type, type_bytesize, array_bytesize);

        return true;
}

struct MemTag* mem_read_var_memtag(const char* name,
                                   const enum MemTagType type,
                                   const size_t index)
{
        struct MemTag* p = get_ptr_var(name);
        if (p == NULL) {
                if (mem_create_var(name, type, index) == false) {
                        printf("err: mem_read_var_memtag(), get_ptr_var() or mem_create_var()\n");
                        exit(1);
                }

                p = get_ptr_var(name);
        }

        p->index = index;
        return p;
}

struct Complex mem_read_var_value(const char* name,
                                  const enum MemTagType type,
                                  const size_t index)
{
        struct MemTag* p = mem_read_var_memtag(name, type, index);

        if (p->type != MTT_COMPVAL) {
                printf("err: mem_read_var_value(), p->type\n");
                exit(1);
        }

        struct Complex* tmp = (struct Complex*)(p->address);
        return tmp[p->index];
}

int mem_push_overlide(void)
{
        int err = stack_push(mem_stack, (void*)&reflist_head);
        if (err) {
                printf("err: mem_push_overlide()\n");
                exit(1);
        }

        return 0;
}

int mem_pop_overlide(void)
{
        int err = stack_pop(mem_stack, (void*)&reflist_head);
        if (err) {
                printf("err: mem_pop_overlide()\n");
                exit(1);
        }

        return 0;
}
