#include "../scan.h"
#include "../compiletimeassert.h"

size_t scan_pb_type1_double(const char* in,size_t len,double* d) {
  size_t n;
  union {
    double d;
    uint64_t u;
  } u;
  compiletimeassert(sizeof(double)==8);
  n=scan_pb_type1_fixed64(in,len,&u.u);
  if (n) *d=u.d;
  return n;
}
