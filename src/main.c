#include <blike.h>

#include "mem.h"
#include "jmptbl.h"

#include "func_putpixel.h"

extern FILE* yyin;
extern FILE* yyout;

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

        start_create_jmptbl();
        while (yylex() != 0) {
        }


        fseek(yyin, 0, SEEK_SET);
        yyrestart(yyin);

        start_main_process();
        yyparse();

        mem_close();
        jmptbl_close();
        return 0;
}

