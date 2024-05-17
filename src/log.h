#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <errno.h>

#define ERR(str) \
fprintf(stderr, "ERR: %s: %s: %d: %s: errno: %s\n", __FILE__, __func__, __LINE__, str, strerror(errno));

#define LOG(str) \
fprintf(stderr, "LOG: %s: %s: %d: %s\n", __FILE__, __func__, __LINE__, str);

#endif /* ifndef LOG_H */
