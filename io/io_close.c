#include "../io_internal.h"
#if ((defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__) && !defined(__MSYS__))
#include <io.h>
#else
#include <unistd.h>
#endif
#include <sys/types.h>
#if ((defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__) && !defined(__MSYS__))
#include <windows.h>
#else
#include <sys/mman.h>
#endif

extern void io_dontwantread_really(int64 d,io_entry* e);
extern void io_dontwantwrite_really(int64 d,io_entry* e);

void io_close(int64 d) {
  io_entry* e;
  if ((e=iarray_get(&io_fds,d))) {
    e->inuse=0;
    e->cookie=0;
    if (e->kernelwantread) io_dontwantread_really(d,e);
    if (e->kernelwantwrite) io_dontwantwrite_really(d,e);
    if (e->mmapped) {
#if ((defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__) && !defined(__MSYS__))
      UnmapViewOfFile(e->mmapped);
      CloseHandle(e->mh);
#else
      munmap(e->mmapped,e->maplen);
#endif
      e->mmapped=0;
    }
  }
  close(d);
}
