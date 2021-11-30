#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <math.h>

#include "utils.h"
#include "bitmap.h"
#include "error.h"
#include "balloc.h"
typedef struct {
  void* address;
  int upper;
  BitMap bMap;
  int lower;
} *Rep;

extern Balloc bnew(unsigned int size, int l, int u){
    unsigned int eSize = e2size(size2e(size));
    unsigned int e = size2e(eSize);
    void* mapMemory=mmalloc(eSize);

    Rep r=(Rep)mmalloc(sizeof(*r));
    BitMap map = bitmapnew(size, e);
    r->bMap = map;

    if (!r) ERROR("malloc() failed");
    r->address=mapMemory;
    r->upper=u;
    r->lower=l;
    return (Balloc)r;
}
extern void *balloc(Balloc pool, unsigned int size){
    Rep r = (Rep)pool;
    void *memory = r->address;
    
}
extern void bfree(Balloc pool, void *mem){

}
extern unsigned int bsize(Balloc pool, void *mem){

}

extern void bprint(Balloc pool){

}