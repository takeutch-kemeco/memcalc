/* mem.h
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

#include <stddef.h>
#include <stdbool.h>
#include "complex.h"

#ifndef __MEM_H__
#define __MEM_H__

enum MemTagType {
        MTT_NOT_FOUND = 0,
        MTT_COMPVAL,
        MTT_FUNCPTR,
        MTT_VARPTR,
};

struct MemTag {
        enum MemTagType type;
        void*  address;
        size_t type_bytesize;
        size_t array_bytesize;
        size_t index;
};

void mem_init(void);
void mem_close(void);

bool mem_create_var(const char* name,
                    const enum MemTagType type,
                    size_t index_len);

struct MemTag* mem_read_var_memtag(const char* name, const size_t index);

int mem_push_overlide(void);
int mem_pop_overlide(void);

#endif /* __MEM_H__ */
