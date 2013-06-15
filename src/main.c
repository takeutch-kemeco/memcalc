#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "mem.h"
#include "jmptbl.h"

#include "func_putpixel.h"

extern FILE* yyin;
extern FILE* yyout;

int main(int argc, char** argv)
{
        if (argc == 1) {
                print_usage();
                exit(1);
        }

        yyin = fopen(argv[1], "rt");
        if (yyin == NULL) {
                print_file_open_err(argv[1]);
                print_usage();
                exit(1);
        }

        yyout = fopen("/dev/null", "w");
        if (yyout == NULL) {
                print_file_open_err("/dev/null");
                print_usage();
                exit(1);
        }

        mem_init();
        jmptbl_init();

        init_func_all();

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
