#include "../fmt.h"

size_t fmt_longlong(char *dest,signed __int64 i) {
  if (i<0) {
    if (dest) *dest++='-';
    return fmt_ulonglong(dest,(unsigned __int64)-i)+1;
  } else
    return fmt_ulonglong(dest,(unsigned __int64)i);
}
