#include "../scan.h"
#include "../rangecheck.h"

size_t scan_pb_type2_stringlen(const char* in,size_t len,const char** string, size_t* slen) {
  unsigned __int64 l;
  size_t n=scan_varint(in,len,&l);
  if (n 
      //&& !assign(*slen,l)
      ) {
    *string=in+n;
  }
  return n;
}
