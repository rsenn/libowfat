#include <sys/types.h>
#if !(defined(_WIN32) || defined(_WIN64))
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include "../windoze.h"
#include "../socket.h"

int socket_mcttl4(int s,char TTL)
{
  return winsock2errno(setsockopt(s,IPPROTO_IP,IP_MULTICAST_TTL,&TTL,sizeof TTL));
}

