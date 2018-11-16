#include <sys/types.h>
#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#else
#include <unistd.h>
#endif
#ifdef __MINGW32__
#include <windows.h>
#else
#include <sys/mman.h>
#endif
#include "open.h"
#include "mmap.h"

int mmap_unmap(const char* mapped,size_t maplen) {
#ifdef __MINGW32__
  (void)maplen;
  return UnmapViewOfFile(mapped)?0:-1;
#else
  return maplen ? munmap((char*)mapped,maplen) : 0;
#endif
}
