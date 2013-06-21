/* mes.c
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
#include "config.h"

void print_usage(void)
{
        printf("使用法: %s [入力ファイル]\n"
               "\n"
               "%s version %s\n"
               "\n"
               "repository: <https://github.com/takeutch-kemeco/memcalc>\n"
               "bug report: <%s>\n",
               PACKAGE_NAME,
               PACKAGE_NAME, VERSION,
               PACKAGE_BUGREPORT);
}

void print_file_open_err(char* s)
{
        printf("ファイル %s のオープンでエラーが発生しました。中止します。\n", s);
}

