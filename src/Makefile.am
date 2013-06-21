bin_PROGRAMS = memcalc
memcalc_SOURCES = main.c                                \
                  memcalc.flex.l memcalc.bison.y        \
                  mem.c mem.h                     	\
                  jmptbl.c jmptbl.h                     \
                  func_putpixel.c func_putpixel.h       \
                  mes.c mes.h				\
                  complex.c complex.h

LFLAGS = -Cf -8
YFLAGS = -dv
CFLAGS = `pkg-config blike --cflags` -g
LIBS = `pkg-config blike --libs` -lm -lfl

