/* jmptbl.c
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

#define JT_NAME_MAX 0x100
struct JmpTbl {
        char name[JT_NAME_MAX];
        long fpos;
};

#define JTLST_MAX 0x1000
static struct JmpTbl* jtlst = NULL;
static u_int jtlst_head = 0;

void jmptbl_init(void)
{
        jtlst_head = 0;

        if (jtlst != NULL)
                return;

        jtlst = malloc(sizeof(*jtlst) * JTLST_MAX);

        int i;
        for (i = 0; i < JTLST_MAX; i++) {
                struct JmpTbl* p = &jtlst[i];
                p->name[0] = '\0';
        }
}

void jmptbl_close(void) {
        free(jtlst);
        jtlst = NULL;
}

long jmptbl_seek(char* name)
{
        size_t name_len = strlen(name);

        int i;
        for (i = 0; i < jtlst_head; i++) {
                struct JmpTbl* p = &jtlst[i];
                if (strlen(p->name) == name_len) {
                        if (strcmp(p->name, name) == 0) {
                                return p->fpos;
                        }
                }
        }

        return -1;
}

void jmptbl_add(char* name, long fpos)
{
        if (jmptbl_seek(name) == -1) {
                struct JmpTbl* p = &jtlst[jtlst_head];
                strcpy(p->name, name);
                p->fpos = fpos;

                jtlst_head++;
        }
}
