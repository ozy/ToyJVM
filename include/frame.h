#ifndef FRAME_H_
#define FRAME_H_
#include <inttypes.h>
#include "stack.h"

typedef int_least32_t LocalVariable;
// A single local variable can hold a value of type boolean, byte, char, short, int, float, reference, or returnAddress. 
// A pair of local variables can hold a value of type long or double. 

typedef int_least64_t Operand;
// Each entry on the operand stack can hold a value of any Java Virtual Machine type, including a value of type long or type double. 

typedef struct Frame{
    LocalVariable* localVariables;
    Stack* operandStack;
    void* runTimeConstantPoolRef; 
    //A run-time constant pool is a per-class or per-interface run-time representation of the constant_pool table in a class file (§4.4).
    //It contains several kinds of constants, ranging from numeric literals known at compile-time to method and field references that must be resolved at run-time.
} Frame;

#endif