bin_PROGRAMS = test_array test_mem test_que test_stack                  \
               test_node test_node2 test_node3                          \
               test_calcnode_expression

test_array_SOURCES = test_array.c ../array.c ../array.h

test_mem_SOURCES = test_mem.c ../mem.c ../mem.h ../stack.c ../stack.h   \
                   ../complex.c ../complex.h ../array.c ../array.h

test_node_SOURCES = test_node.c ../node.c ../node.h ../array.c ../array.h

test_node2_SOURCES = test_node2.c ../node.c ../node.h ../array.c ../array.h

test_node3_SOURCES = test_node3.c ../node.c ../node.h ../array.c ../array.h

test_que_SOURCES = test_que.c ../que.c ../que.h ../array.c ../array.h

test_stack_SOURCES = test_stack.c ../stack.c ../stack.h ../array.c ../array.h

test_calcnode_expression_SOURCES = test_calcnode_expression.c           \
                                   ../node.c ../node.h                  \
                                   ../complex.c ../complex.h            \
                                   ../array.c ../array.h                \
                                   ../memcalc.bison.h                   \
                                   ../calcnode.c ../calcnode.h          \
                                   ../calcnode_expression.c ../calcnode_expression.h

CFLAGS = -g -DDEBUG
LIBS = -lm
