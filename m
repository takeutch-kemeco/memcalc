rm -f memcalc memcalc.output memcalc.tab.c memcalc.tab.h memcalc.yy.c
bison -dv memcalc.y
flex -o memcalc.yy.c memcalc.l
gcc -o memcalc memcalc.tab.c memcalc.yy.c mem.c jmptbl.c -lm -lfl
