#ifndef STACK_H_
#define STACK_H_
#include <stdint.h>

typedef struct Stack{
    unsigned char* stack;
    uint64_t top;
    uint64_t maxSize;
}Stack;

#endif