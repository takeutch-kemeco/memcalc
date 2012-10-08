bin_PROGRAMS = memcalc
memcalc_SOURCES = memcalc.flex.l memcalc.bison.y        \
                  mem.c mem.h                     	\
                  jmptbl.c jmptbl.h                     \
                  func_putpixel.c func_putpixel.h       \
                  mes.c mes.h

LFLAGS = -Cf -8
YFLAGS = -dv
CFLAGS = -g
LIBS = -lm -lfl
