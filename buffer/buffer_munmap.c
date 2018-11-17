#include "../buffer.h"
#if ((defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__) && !defined(__MSYS__))
#include <windows.h>
#else
#include <sys/mman.h>
#endif

void buffer_munmap(void* buf) {
  buffer* b=(buffer*)buf;
#if ((defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__) && !defined(__MSYS__))
  UnmapViewOfFile(b->x);
#else
  munmap(b->x,b->a);
#endif
}
