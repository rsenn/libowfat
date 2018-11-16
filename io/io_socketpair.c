#include "../io_internal.h"
#include "../windoze.h"
#include <sys/types.h>
#if ((defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__) && !defined(__MSYS__))
#include <io.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include <errno.h>

#if (((defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__) && !defined(__MSYS__)))

#define socketpair(af, sock, proto, fds) windoze_socketpair(fds, TRUE)
/* 
 *   If make_overlapped is nonzero, both sockets created will be usable for
 *   "overlapped" operations via WSASend etc.  If make_overlapped is zero,
 *   socks[0] (only) will be usable with regular ReadFile etc., and thus
 *   suitable for use as stdin or stdout of a child process.  Note that the
 *   sockets must be closed with closesocket() regardless.
 */
int
windoze_socketpair(SOCKET socks[2], int make_overlapped) {
  union {
    struct sockaddr_in inaddr;
    struct sockaddr addr;
  } a;
  SOCKET listener;
  int e;
  int addrlen = sizeof(a.inaddr);
  DWORD flags = (make_overlapped ? WSA_FLAG_OVERLAPPED : 0);
  int reuse = 1;

  if(socks == 0) {
    WSASetLastError(WSAEINVAL);
    return SOCKET_ERROR;
  }
  socks[0] = socks[1] = -1;

  listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(listener == -1)
    return SOCKET_ERROR;

  memset(&a, 0, sizeof(a));
  a.inaddr.sin_family = AF_INET;
  a.inaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  a.inaddr.sin_port = 0;

  for(;;) {
    if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse)) == -1)
      break;
    if(bind(listener, &a.addr, sizeof(a.inaddr)) == SOCKET_ERROR)
      break;

    memset(&a, 0, sizeof(a));
    if(getsockname(listener, &a.addr, &addrlen) == SOCKET_ERROR)
      break;
    // win32 getsockname may only set the port number, p=0.0005.
    // ( http://msdn.microsoft.com/library/ms738543.aspx ):
    a.inaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    a.inaddr.sin_family = AF_INET;

    if(listen(listener, 1) == SOCKET_ERROR)
      break;

    socks[0] = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, flags);
    if(socks[0] == -1)
      break;
    if(connect(socks[0], &a.addr, sizeof(a.inaddr)) == SOCKET_ERROR)
      break;

    socks[1] = accept(listener, NULL, NULL);
    if(socks[1] == -1)
      break;

    closesocket(listener);
    return 0;
  }

  e = WSAGetLastError();
  closesocket(listener);
  closesocket(socks[0]);
  closesocket(socks[1]);
  WSASetLastError(e);
  socks[0] = socks[1] = -1;
  return SOCKET_ERROR;
}
#endif

int
io_socketpair(int64* d) {
  int fds[2];
  __winsock_init();
  if(socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == -1)
    if(socketpair(AF_INET6, SOCK_STREAM, IPPROTO_TCP, fds) == -1)
      if(socketpair(AF_INET, SOCK_STREAM, IPPROTO_TCP, fds) == -1)
        return 0;
  if(io_fd(fds[1]) && io_fd(fds[0])) {
    d[0] = fds[0];
    d[1] = fds[1];
    return 1;
  }
  io_close(fds[1]);
  io_close(fds[0]);
  return 0;
}
