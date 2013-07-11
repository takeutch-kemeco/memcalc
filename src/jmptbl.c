/* jmptbl.c
 * Copyright (C) 2012, 2013 Takeutch Kemeco
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
#include <stdint.h>
#include "array.h"
#include "node.h"
#include "jmptbl.h"

#define DEFAULT_JTLST_LEN 0x100

static struct Array* jmptbl = NULL;
static size_t jmptbl_head;

static void* unit_constructor(void)
{
        struct JmpTblUnit* tmp = malloc(sizeof(*tmp));
        if (tmp == NULL) {
                printf("err: jmptbl.c, unit_constructor()\n");
                exit(1);
        }

        tmp->name = NULL;
        tmp->name_len = 0;
        tmp->fpos = 0;
        tmp->pcpos = 0;
        tmp->node = NULL;

        return (void*)tmp;
}

static int unit_destructor(void* a)
{
        /* 注意: name と node の参照先は他でも使ってるので開放してはならない */

        free(a);
        return 0;
}

static int unit_copy(void* dst, void* src)
{
        *((struct JmpTblUnit*)dst) = *((struct JmpTblUnit*)src);
        return 0;
}

void jmptbl_init(void)
{
        jmptbl_head = 0;

        if (jmptbl != NULL) {
                printf("err: すでに初期化済みで稼働中の jmptbl を初期化しようとしました\n");
                exit(1);
        } else {
                jmptbl = array_new(DEFAULT_JTLST_LEN, unit_constructor, unit_destructor, unit_copy);
                if (jmptbl == NULL) {
                        printf("err: jmptbl_init(), array_new() jmptbl\n");
                        exit(1);
                }
        }
}

void jmptbl_close(void) {
        array_free(jmptbl);
        jmptbl = NULL;
}

static int jmptbl_seek_all(char* name, struct JmpTblUnit* dst)
{
        const size_t name_len = strlen(name);

        size_t i;
        for (i = 0; i < jmptbl_head; i++) {
                int err = array_read_unit(jmptbl, i, dst);
                if (err) {
                        printf("jmptbl.c, jmptbl_seek_all(), array_read_unit()\n");
                        exit(1);
                }

                if (dst->name_len == name_len) {
                        if (strcmp(dst->name, name) == 0) {
                                return 0;
                        }
                }
        }

        return -1;
}

static void jmptbl_extend_x2(void)
{
        if (jmptbl->len <= jmptbl_head) {
                int err = array_extend_x2(jmptbl);
                if (err) {
                        printf("err: jmptbl.c, jmptbl_add_all(), array_extend_x2()\n");
                        exit(1);
                }
        }
}

static void jmptbl_add_all(char* name,
                           const int64_t fpos,
                           const int64_t pcpos,
                           struct Node* node)
{
        struct JmpTblUnit tmp;
        const int not_found = jmptbl_seek_all(name, &tmp);

        if (not_found) {
                jmptbl_extend_x2();

                tmp.name = name;
                tmp.name_len = strlen(name);
                tmp.fpos = fpos;
                tmp.pcpos = pcpos;
                tmp.node = node;


        } else {
                if (fpos != -1)
                        tmp.fpos = fpos;

                if (pcpos != -1)
                        tmp.pcpos = pcpos;

                if (node != NULL)
                        tmp.node = node;
        }

        int err = array_write_unit(jmptbl, jmptbl_head, (void*)&tmp);
        if (err) {
                printf("jmptbl.c, jmptbl_add_all(), array_write_unit()\n");
                exit(1);
        }

        jmptbl_head++;
}

int64_t jmptbl_seek_fpos(char* name)
{
        struct JmpTblUnit tmp;
        const int not_found = jmptbl_seek_all(name, &tmp);

        if (not_found)
                return -1;
        else
                return tmp.fpos;
}

int64_t jmptbl_seek_pcpos(char* name)
{
        struct JmpTblUnit tmp;
        const int not_found = jmptbl_seek_all(name, &tmp);

        if (not_found)
                return -1;
        else
                return tmp.pcpos;
}

struct Node* jmptbl_seek_node(char* name)
{
        struct JmpTblUnit tmp;
        const int not_found = jmptbl_seek_all(name, &tmp);

        if (not_found)
                return NULL;
        else
                return tmp.node;
}

void jmptbl_add_fpos(char* name, const int64_t fpos)
{
        jmptbl_add_all(name, fpos, -1, NULL);
}

void jmptbl_add_pcpos(char* name, const int64_t pcpos)
{
        jmptbl_add_all(name, -1, pcpos, NULL);
}

void jmptbl_add_node(char* name, struct Node* node)
{
        jmptbl_add_all(name, -1, -1, node);
}
