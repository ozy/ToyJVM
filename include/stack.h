#ifndef STACK_H_
#define STACK_H_
#include <stdint.h>
#include "frame.h"

typedef enum StackType{
    TYPE_OPERANDSTACK = sizeof(uint64_t),
    TYPE_JVMSTACK = sizeof(struct Frame)
}StackType;

typedef struct Stack{
    unsigned char* stack;
    StackType stackType;
    uint64_t top;
    uint64_t maxSize;
}Stack;

Stack initStack(uint64_t numItems, StackType stackType);
// size is given as numItems here

void* popStack(Stack* stack);
void pushStack(void* val, Stack* stack);
void* peekStack(Stack* stack);
#endif 