/* jmptbl.h
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

#include <stdint.h>

#ifndef __JMPTBL_H__
#define __JMPTBL_H__

struct JmpTblUnit {
        char* name;
        size_t name_len;
        int64_t fpos;
        int64_t pcpos;
        struct Node* node;
};

void jmptbl_init(void);
void jmptbl_close(void);

int64_t jmptbl_seek_fpos(char* name);
int64_t jmptbl_seek_pcpos(char* name);
struct Node* jmptbl_seek_node(char* name);

void jmptbl_add_fpos(char* name, const int64_t fpos);
void jmptbl_add_pcpos(char* name, const int64_t pcpos);
void jmptbl_add_node(char* name, struct Node* node);

#endif /* __JMPTBL_H__ */
