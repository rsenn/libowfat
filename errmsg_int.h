#include "buffer.h"
#include <stdarg.h>

ssize_t errmsg_puts(int fd,const char* s);
ssize_t errmsg_flush(int fd);
void errmsg_start(int fd);

void errmsg_write(int fd,const char* err,const char* message,va_list list);
