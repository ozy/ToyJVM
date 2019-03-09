#ifndef HEAP_H_
#define HEAP_H_
#include <inttypes.h>
#include <stdlib.h>

typedef struct HeapObject{
    void* addr;
    size_t size;
    uint8_t isUsed;
}HeapObject;

typedef struct Heap{
    uint64_t size;
    uint64_t top;
    HeapObject* heap;
}Heap;

Heap initHeap(size_t size);
void* hGet(uint64_t addr, Heap* heap);
uint64_t hAlloc(size_t size, Heap* heap);
uint64_t hExtend(uint64_t addr, size_t size, Heap* heap);
void* hFree(uint64_t addr, Heap* heap);

#endif