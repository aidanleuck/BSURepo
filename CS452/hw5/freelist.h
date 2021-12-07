#ifndef FREELIST_H
#define FREELIST_H

typedef void *FreeList;

extern FreeList freelistnew(unsigned int size, void *base, int l, int u);

extern void *freelistalloc(FreeList f, void *base, int e, int l, int u);
extern void freelistfree(FreeList f, void *base, void *mem, int e, int l);
extern int freelistsize(FreeList f, void *base, void *mem, int l, int u);

extern void freelistprint(FreeList f, unsigned int size, int l, int u);
extern void printNode(void* node);
extern void split(FreeList f, void *base, void* addr, int* e);

#endif
