#ifndef MALLOC
#include <stdlib.h>
#include "log.h"
#define MALLOC(size, ret) \
({\
 void *p = malloc(size);\
	 if (p == NULL){\
		ERR("malloc");\
		ret;\
	 }\
	 memset(p,0,size);\
	 p;\
 })
#endif /* ifndef MALLOC */

#ifndef REALLOC
#define REALLOC(ptr, size, ret) \
({\
 void *p = realloc(ptr, size);\
	 if (p == NULL){\
		ERR("realloc");\
		ret;\
	 }\
	 p;\
 })
#endif /* ifndef REALLOC */
