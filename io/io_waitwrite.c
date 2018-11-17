#include "../io_internal.h"
#if ((defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__) && !defined(__MSYS__))
#include <io.h>
#else
#include <unistd.h>
#endif
#if ((defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__) && !defined(__MSYS__))
#include <windows.h>
#else
#include <poll.h>
#endif
#include <errno.h>

#if ((defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__) && !defined(__MSYS__))

int64 io_waitwrite(int64 d,const char* buf,int64 len) {
  long r;
  io_entry* e=iarray_get(&io_fds,d);
  if (!e) { errno=EBADF; return -3; }
  if (e->nonblock) {
    unsigned long i=0;
    ioctlsocket(d, FIONBIO, &i);
  }
  r=write(d,buf,len);
  if (e->nonblock) {
    unsigned long i=1;
    ioctlsocket(d, FIONBIO, &i);
  }
  if (r==-1)
    r=-3;
  return r;
}

#else

int64 io_waitwrite(int64 d,const char* buf,int64 len) {
  long r;
  struct pollfd p;
  io_entry* e=iarray_get(&io_fds,d);
  io_sigpipe();
  if (!e) { errno=EBADF; return -3; }
  if (e->nonblock) {
again:
    p.fd=d;
    if (p.fd != d) { errno=EBADF; return -3; }	/* catch overflow */
    p.events=POLLOUT;
    switch (poll(&p,1,-1)) {
    case -1: if (errno==EAGAIN) goto again; return -3;
    }
  }
  r=write(d,buf,len);
  if (r==-1) {
    if (errno==EAGAIN)
      goto again;
    r=-3;
  }
  return r;
}

#endif
