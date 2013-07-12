/* main.c
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

#include <blike0.h>
#include "mem.h"
#include "jmptbl.h"
#include "pc.h"

extern FILE* yyin;
extern FILE* yyout;

extern struct Node* root_node;

blMain()
{
        if (bl_argc == 1) {
                print_usage();
                return 1;
        }

        yyin = fopen(bl_argv[1], "rt");
        if (yyin == NULL) {
                print_file_open_err(bl_argv[1]);
                print_usage();
                return 1;
        }

        yyout = fopen("/dev/null", "w");
        if (yyout == NULL) {
                print_file_open_err("/dev/null");
                print_usage();
                return 1;
        }

        mem_init();
        jmptbl_init();
        pc_init();

        start_pre_process();
        while (yylex() != 0) {
        }

        fseek(yyin, 0, SEEK_SET);
        yyrestart(yyin);

        start_main_process();
        yyparse();

        pc_close();
        jmptbl_close();
        mem_close();

        return 0;
}
