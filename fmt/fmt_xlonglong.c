#include "../fmt.h"

static inline char tohex(char c) {
  return (char)(c>=10?c-10+'a':c+'0');
}

size_t fmt_xlonglong(char *dest,unsigned __int64 i) {
  unsigned __int64 len,tmp;
  /* first count the number of bytes needed */
  for (len=1, tmp=i; tmp>15; ++len) tmp>>=4;
  if (dest)
    for (tmp=i, dest+=len; ; ) {
      *--dest = tohex(tmp&15);
      if (!(tmp>>=4)) break;
    }
  return len;
}
