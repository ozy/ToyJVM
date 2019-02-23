#include "stack.h"
#include "frame.h"
#include <inttypes.h>
#include <stdlib.h>

Stack initStack(uint64_t size, StackType stackType){
    //The maximum depth of the operand stack of a frame is determined at compile-time and 
    //is supplied along with the code for the method associated with the frame
    Stack stack;
    stack.maxSize = size;
    stack.top = 0;
    stack.stackType = stackType;
    stack.stack = malloc(stackType * size); // afaik all stacks in here are frame sized. leak
    return stack;
}
 
void pushStack(void* val, Stack* stack){
    if (stack->top + stack->stackType < stack->maxSize){
        if (stack->stackType == TYPE_JVMSTACK){
            *(Frame*)(&stack->stack[stack->top])=*(Frame*)val;
        }else if(stack->stackType == TYPE_OPERANDSTACK){
            *(uint64_t*)(&stack->stack[stack->top])=*(uint64_t*)val; 
            // this stack is implemented at DWORD size on 32 bit machines
            // QWORD size on 64 bit machines. On the other hand, the values we 
            // push on operand stack always 16 bit.
        } 
        stack->top += stack->stackType;
    }else{
        // todo stackoverflow err
    }
}

void* popStack(Stack* stack){
    if (stack->top - stack->stackType >= 0){
        void* val;
        stack->top -= stack->stackType;
        if (stack->stackType == TYPE_JVMSTACK){
            val = (Frame*)(&stack->stack[stack->top]);
        }else if(stack->stackType == TYPE_OPERANDSTACK){
            val = (uint64_t*)(&stack->stack[stack->top]);
        }
        return val;
    }else{
        // todo stack empty
    }
}