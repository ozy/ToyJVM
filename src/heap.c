#include "heap.h"

Heap initHeap(size_t size){
    Heap heap;
    heap.heap = calloc(size,sizeof(HeapObject));
    heap.size = size;
    heap.top = 1; // 0 is for null object refs. 
    return heap;
}

void* hGet(uint64_t addr, Heap* heap){
    return heap->heap[addr].addr;
}

uint64_t hAlloc(size_t size, Heap* heap){
    for (size_t mem=0; mem<heap->top; mem++){
        // this gets slower and sloooower as the number of object in heap increases.
        if (!heap->heap[mem].isUsed){
            heap->heap[mem].isUsed=1;
            heap->heap[mem].size = size;
            free(heap->heap[mem].addr);
            heap->heap[mem].addr = malloc(heap->heap[mem].size);
            return (mem);
        }
    }
    if(heap->top + 1 > heap->size){
        heap->heap = realloc(heap->heap,heap->size+=1024); // realloc 1024 more
    }
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

void destroyHeap(Heap* heap){
    for(;heap->top > 0; heap->top--)
        free(heap->heap[heap->top -1].addr);
    free(heap->heap);
    free(heap);
}