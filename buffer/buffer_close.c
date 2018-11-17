#include "../buffer.h"
#if ((defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__) && !defined(__MSYS__))
#include <io.h>
#else
#include <unistd.h>
#endif

void buffer_close(buffer* b) {
  if (b->fd > 2) close(b->fd);
  if (b->deinit) b->deinit(b);
}
