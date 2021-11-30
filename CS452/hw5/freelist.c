#include <unistd.h>
#include <string.h>
#include "utils.h"

#include "freelist.h"
#include "utils.h"
typedef struct{
    unsigned int size;
    
}*FreeListNode;
typedef struct {
    FreeListNode head;
}*OrderList;
typedef struct{
    int low;
    OrderList* list;
}*ListRep;

extern FreeList freelistnew(unsigned int size, int l, int u){
    ListRep r = (ListRep) mmalloc(u * sizeof(OrderList));
    return (FreeList)r;
}
extern void *freelistalloc(FreeList f, void *base, int e, int l){
    ListRep r = (ListRep)f;
}
extern void freelistfree(FreeList f, void *base, void *mem, int e, int l){

}
extern int freelistsize(FreeList f, void *base, void *mem, int l, int u){

}
extern void freelistprint(FreeList f, unsigned int size, int l, int u){

}
