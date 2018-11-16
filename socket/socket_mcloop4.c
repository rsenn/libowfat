#include <sys/types.h>
#if !(defined(_WIN32) || defined(_WIN64))
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include "../windoze.h"
#include "../socket.h"

int socket_mcloop4(int s,char loop)
{
  return winsock2errno(setsockopt(s,IPPROTO_IP,IP_MULTICAST_LOOP,&loop,sizeof loop));
}

