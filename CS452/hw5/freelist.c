#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"

#include "freelist.h"
#include "utils.h"
#include "bitmap.h"
typedef struct{
    void* head;
    BitMap map;
}FreeListSpace;
typedef struct{
    void* next;
}Next;

static FreeListSpace list[];
extern FreeList freelistnew(unsigned int size, void* base, int l, int u){
    int numLists = u-l;
    memset(&list, 0, sizeof(FreeListSpace) *numLists);

    for(int i =0; i <= numLists; i++){
        int levelSize = size2e(i + l);
        list[i].map = bitmapnew(size, levelSize);
    }
    list[numLists].head = base;
    return (FreeList)list;
}
extern void *freelistalloc(FreeList f, void *base, int e, int l, int u){
    FreeListSpace *rep = (FreeListSpace *)f;
    
    void *allocatedAddress = NULL;

    int startLevel = e - l;
    int endLevel = u - l;
    int index = startLevel;

    FreeListSpace currSpace = rep[startLevel];
    
    // We need u so we know if we don't have enough memory to return to user
    while (currSpace.head == NULL && index <= endLevel)
    {
        currSpace = rep[index];
        index++;
    }
    int foundIndex = startLevel == index ? startLevel : index - 1;
    allocatedAddress = currSpace.head;
    if (foundIndex >= startLevel && rep[foundIndex].head != NULL)
    {
        for (int i = foundIndex + l; i > e; i--)
        {
            int fListIndex = i - l;
            void *buddy = buddyinv(base, allocatedAddress, i-1);
            Next *nextPntr = buddy;

            void *currentHead = rep[fListIndex - 1].head;
            rep[fListIndex - 1].head = buddy;
            nextPntr->next = currentHead;
        }
    }
    allocatedAddress = currSpace.head;
    Next *initialBlock = rep[foundIndex].head;
    rep[foundIndex].head = initialBlock->next;

    bitmapset(rep[startLevel].map, base, allocatedAddress, e);
    return allocatedAddress;
}
extern void printNode(void* node){
  
}
extern void freelistfree(FreeList f, void *base, void *mem, int e, int l){
    
}
extern int freelistsize(FreeList f, void *base, void *mem, int l, int u)
{
    FreeListSpace *rep = (FreeListSpace *)f;  
    int size = 0;
    int numLists = u-l;
    int counter = 0;

    while(counter <= u){
        int e = counter + l;
        if(bitmaptst(rep[counter].map, base, mem, e)){
            size = e2size(e);
            break;
        }
        counter++;
    }
    return size;
}
extern void freelistprint(FreeList f, unsigned int size, int l, int u){
   
   
}

