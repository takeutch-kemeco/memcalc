bin_PROGRAMS = memcalc
memcalc_SOURCES = memcalc.flex.l memcalc.bison.y        \
                  mem.c mem.h                     	\
                  jmptbl.c jmptbl.h                     \
                  func_putpixel.c func_putpixel.h       \
                  mes.c mes.h

LFLAGS = -Cf
YFLAGS = -dv
CFLAGS = -O2 -mtune=native -g
LIBS = -lm -lfl
