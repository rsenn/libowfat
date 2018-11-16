
#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#else
#include <unistd.h>
#endif
#include "../io_internal.h"

void io_setcookie(int64 d,void* cookie) {
  io_entry* e;
  if ((e=iarray_get(&io_fds,d)))
    e->cookie=cookie;
}
