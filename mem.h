#include <string.h>
#include <sys/types.h>

#ifndef __MEM_H__
#define __MEM_H__

struct MemTag {
	void*  address;
	size_t bytesize;
};

void mem_init(void);
void mem_close(void);

void push_var(char* name, size_t bytesize);
void pop_var(void);
struct MemTag* get_ptr_var(char* name);

#endif /* __MEM_H__ */
