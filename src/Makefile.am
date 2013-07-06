bin_PROGRAMS = memcalc
memcalc_SOURCES = main.c                                        \
                  memcalc.flex.l memcalc.bison.y                \
                  mem.c mem.h                                   \
                  jmptbl.c jmptbl.h                             \
                  func_putpixel.c func_putpixel.h               \
                  mes.c mes.h                                   \
                  complex.c complex.h                           \
                  pc.c pc.h                                     \
                  stack.c stack.h                               \
                  que.c que.h                                   \
                  array.c array.h                               \
                  node.c node.h                                 \
                  calcnode.c calcnode.h                         \
                  calcnode_expression.c calcnode_expression.h   \
                  calcnode_function.c calcnode_function.h

LFLAGS = -Cf -8
YFLAGS = -dv
CFLAGS = `pkg-config blike --cflags` -g
LIBS = `pkg-config blike --libs` -lm -lfl
