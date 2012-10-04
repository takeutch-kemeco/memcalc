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

#include <string.h>
#include <sys/types.h>

#ifndef __MEM_H__
#define __MEM_H__

struct MemTag {
        void*  address;
        size_t bytesize;
};

void mem_init(void);
void mem_close(void);

void push_var(char* name, size_t bytesize);
void pop_var(void);
struct MemTag* get_ptr_var(char* name);

#endif /* __MEM_H__ */