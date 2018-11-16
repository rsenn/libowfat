#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#else
#include <unistd.h>
#endif
#include "buffer.h"
#ifdef __MINGW32__
#include <io.h>
#endif

char buffer_1_space[BUFFER_INSIZE];
static buffer it = BUFFER_INIT(write,1,buffer_1_space,sizeof buffer_1_space);
buffer *buffer_1 = &it;

