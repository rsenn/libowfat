#if ((defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__) && !defined(__MSYS__))
#include <io.h>
#else
#include <unistd.h>
#endif
#include "../io_internal.h"

int io_pipe(int64* d) {
#if ((defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__) && !defined(__MSYS__))
  HANDLE fds[2];
  if (CreatePipe(fds,fds+1,0,0)==0)
    return 0;
#else
  int fds[2];
  if (pipe(fds)==-1)
    return 0;
#endif
  if (io_fd((int64)fds[1]) && io_fd((int64)fds[0])) {
    d[0]=(int64)fds[0];
    d[1]=(int64)fds[1];
    return 1;
  }
  io_close((int64)fds[1]);
  io_close((int64)fds[0]);
  return 0;
}
