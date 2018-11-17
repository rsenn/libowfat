#if ((defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__) && !defined(__MSYS__))
#include <io.h>
#else
#include <unistd.h>
#endif
#include "../io_internal.h"

void* io_getcookie(int64 d) {
  io_entry* e;
  e=iarray_get(&io_fds,d);
  return e?e->cookie:0;
}
