#include <stdlib.h>
#if ((defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__) && !defined(__MSYS__))
#include <io.h>
#else
#include <unistd.h>
#include <sys/mman.h>
#endif
#include "../iarray.h"

static void freechain(iarray_page* p,size_t pagesize) {
  while (p) {
    iarray_page* n=p->next;
#if ((defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__) && !defined(__MSYS__))
    free(p);
#else
    munmap(p,pagesize);
#endif
    p=n;
  }
}

void iarray_free(iarray* ia) {
  size_t i;
  for (i=0; i<sizeof(ia->pages)/sizeof(ia->pages[0]); ++i) {
    freechain(ia->pages[i],ia->bytesperpage);
    ia->pages[i]=0;
  }
}
