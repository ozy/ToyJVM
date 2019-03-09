#include "heap.h"

Heap initHeap(size_t size){
    Heap heap;
    heap.heap = malloc(size);
    heap.size = size;
    heap.top = 1; // 0 is for null object refs. 
    return heap;
}

void* hGet(uint64_t addr, Heap* heap){
    return heap->heap[addr].addr;
}

uint64_t hAlloc(size_t size, Heap* heap){
    heap->heap[heap->top].isUsed = 1;
    heap->heap[heap->top].size = size;
    heap->heap[heap->top].addr = malloc(size);
    return (heap->top++);
}

uint64_t hExtend(uint64_t addr, size_t size, Heap* heap){
    heap->heap[addr].size += size;
    heap->heap[addr].addr = realloc(heap->heap[addr].addr,heap->heap[addr].size);
    return addr;
}

void* hFree(uint64_t addr, Heap* heap){
    heap->heap[addr].isUsed = 0;
    return NULL;
}
