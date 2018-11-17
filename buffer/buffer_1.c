#if ((defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__) && !defined(__MSYS__))
#undef __STDC__
#include <io.h>
#define write _write
#else
#include <unistd.h>
#endif
#include "../buffer.h"

char buffer_1_space[BUFFER_INSIZE];
static buffer it = BUFFER_INIT(write,1,buffer_1_space,sizeof buffer_1_space);
buffer *buffer_1 = &it;

