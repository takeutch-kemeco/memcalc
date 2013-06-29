#include <stdio.h>
#include "mem.h"

int t = 123;

void f(void)
{
        char s[] = "sss";

        if (mem_create_var(s, 100) == false) {
                printf("err: mem_create_var(s, 100)\n");
        }

        struct MemTag* p = mem_read_var_memtag(s, 0);

        printf("p=[%p, %d],\t", p, *((int*)p->address));

        *((int*)(p->address)) = t++;

        printf("p=[%p, %d]\n", p, *((int*)p->address));
}

int main(int argc, char** argv)
{
        mem_init();

        f();
        mem_push_overlide();
        f();
        mem_push_overlide();
        f();

        mem_pop_overlide();
        f();
        mem_pop_overlide();
        f();
        mem_pop_overlide();
        f();

        mem_close();
}
