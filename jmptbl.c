#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define JT_NAME_MAX 0x100
struct JmpTbl {
	char name[JT_NAME_MAX];
	long fpos;
};

#define JTLST_MAX 0x1000
static struct JmpTbl* jtlst = NULL;
static u_int jtlst_head = 0;

void jmptbl_init(void)
{
	jtlst_head = 0;

	if (jtlst != NULL)
		return;

	jtlst = malloc(sizeof(*jtlst) * JTLST_MAX);

	int i;
	for (i = 0; i < JTLST_MAX; i++) {
		struct JmpTbl* p = &jtlst[i];
		p->name[0] = '\0';
	}
}

void jmptbl_close(void) {
	free(jtlst);
	jtlst = NULL;
}

long jmptbl_seek(char* name)
{
	size_t name_len = strlen(name);

	int i;
	for (i = 0; i < jtlst_head; i++) {
		struct JmpTbl* p = &jtlst[i];
		if (strlen(p->name) == name_len) {
			if (strcmp(p->name, name) == 0) {
				return p->fpos;
			}
		}
	}

	return -1;
}

void jmptbl_add(char* name, long fpos)
{
	if (jmptbl_seek(name) == -1) {
		struct JmpTbl* p = &jtlst[jtlst_head];
		strcpy(p->name, name);
		p->fpos = fpos;

		jtlst_head++;

		printf("jmptbl [%s][%p] = [%d]\n", name, &(p->fpos), p->fpos);
	}
}
