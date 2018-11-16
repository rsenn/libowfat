#include <sys/types.h>
#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

int main() {
  socklen_t t;
  (void)t;
  return 0;
}
