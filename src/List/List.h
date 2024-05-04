#ifndef LIST_H
#define LIST_H

#include <stdbool.h>


typedef struct sList sList_t;
typedef struct sNode sNode_t;
typedef void* ListIterator_t;
typedef bool(*fListCompare)(void* dataA, void* dataB);
typedef bool(*fListFreeNode)(sNode_t* node);

sList_t* ListCreate(int dataSize, fListCompare compareCallback, fListFreeNode freeNodeCallback);

ListIterator_t ListGetIterator(sList_t*);
bool ListFreeIterator(ListIterator_t iterator);

int ListGetCount(sList_t*);
int ListGetDataSize(sList_t*);

bool ListRemove(sList_t*, void*);
bool ListAppend(sList_t*, void*);
bool ListPrepend(sList_t*, void*);

bool ListInsertAfter(sList_t*, void*, void*);
bool ListInsertBefore(sList_t*, void*, void*);

int ListContains(sList_t*, void*);
int ListRemoveDuplicates(sList_t*);

bool ListFree(sList_t*);

#endif