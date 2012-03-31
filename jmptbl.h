#include <stdio.h>

#ifndef __JMPTBL_H__
#define __JMPTBL_H__

void jmptbl_init(void);
void jmptbl_close(void);

long jmptbl_seek(char* name);
void jmptbl_add(char* name, long pos);

#endif /* __JMPTBL_H__ */
