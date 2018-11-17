#include <sys/types.h>
#if !(((defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__) && !defined(__MSYS__)))
#include <sys/socket.h>
#include <net/if.h>
#endif
#include "../socket.h"
#include "haven2i.h"

#ifdef HAVE_N2I
/* legacy BSD name */
#ifndef IF_NAMESIZE
#define IF_NAMESIZE IFNAMSIZ
#endif

static char ifname[IF_NAMESIZE];

const char* socket_getifname(uint32 _interface) {
  char *tmp=if_indextoname(_interface,ifname);
  if (tmp)
    return tmp;
  else
    return "[unknown]";
}
#else
const char* socket_getifname(uint32 _interface) {
  return "[unknown]";
}
#endif
