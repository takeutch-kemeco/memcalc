/* pc.c
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

struct PCTbl {
        uint32_t fpos;
};

#define PCTBL_MAX 0x1000
static struct PCTbl* pctbl = NULL;
static uint32_t pctbl_head = 0;

void pc_init(void)
{
        pctbl_head = 0;

        if (pctbl != NULL)
                return;

        pctbl = malloc(sizeof(*pctbl) * PCTBL_MAX);
}

void pc_close(void) {
        free(pctbl);
        pctbl = NULL;
}

void pc_push(const uint32_t fpos)
{
        pctbl[pctbl_head].fpos = fpos;
        pctbl_head++;
}

uint32_t pc_pop(void)
{
        if (pctbl_head <= 0) {
                printf("err: pc_pop(), pctbl_head\n");
                exit(1);
        }

        pctbl_head--;
        return pctbl[pctbl_head].fpos;
}
