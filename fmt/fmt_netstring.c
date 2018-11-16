#include "../likely.h"
#include "../fmt.h"
#include "../rangecheck.h"
#include <string.h>

size_t fmt_netstring(char* dest,const char* src,size_t len) {
  size_t n=fmt_ulong(NULL,len);
  size_t m;
  int add = 0;
  
  //add_of(add,size_t,m,len,n+2);
  if (!range_validbuf(src,len) || add) return 0;	/* n came from fmt_ulong, so it is a very small number, below 50 */
  if (!dest) return m;
  fmt_ulong(dest,len); dest+=n;
  *dest++=':';
  memcpy(dest,src,len);
  dest[len]=',';
  return m;
}
