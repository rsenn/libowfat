#define _FILE_OFFSET_BITS 64
#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#else
#include <unistd.h>
#endif
#include <fcntl.h>
#include "../open.h"

#ifndef O_NDELAY
#define O_NDELAY 0
#endif

int open_rw(const char *filename) {
  return open(filename,O_RDWR|O_CREAT|O_NDELAY,0644);
}
