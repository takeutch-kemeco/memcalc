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
                  node_debug.c node_debug.h                     \
                  calcnode.c calcnode.h                         \
                  calcnode_expression.c calcnode_expression.h   \
                  calcnode_function.c calcnode_function.h       \
                  calcnode_bl_function.c calcnode_bl_function.h \
                  calcnode_comparison.c calcnode_comparison.h   \
                  calcnode_read_variable.c calcnode_read_variable.h

LFLAGS = -Cf -8
YFLAGS = -dv
CFLAGS = `pkg-config blike --cflags` -g
LIBS = `pkg-config blike --libs` -lm -lfl
