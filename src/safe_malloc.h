#include <stdio.h>
#ifndef MALLOC
#define MALLOC(size, ret) \
({\
 void *p = malloc(size);\
	 if (p == NULL){\
		perror("malloc");\
		ret;\
	 }\
	 p;\
 })
#endif /* ifndef MALLOC */

#ifndef REALLOC
#define REALLOC(ptr, size, ret) \
({\
 void *p = realloc(ptr, size);\
	 if (p == NULL){\
		perror("malloc");\
		ret;\
	 }\
	 p;\
 })
#endif /* ifndef REALLOC */
