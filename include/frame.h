#ifndef FRAME_H_
#define FRAME_H_
#include <inttypes.h>
//#include "stack.h"
#include "attribute.h"
#include "constantPool.h"

typedef struct ClassFile ClassFile;
typedef struct Machine Machine;
typedef uint64_t LocalVariable;
// A single local variable can hold a value of type boolean, byte, char, short, int, float, reference, or returnAddress. 
// A pair of local variables can hold a value of type long or double. 
// But we wont implement this, instead a local variable in our jvm can hold up to 64 bits.
typedef uint64_t Operand;
// Each entry on the operand stack can hold a value of any Java Virtual Machine type, including a value of type long or type double. 

typedef struct Frame{
    LocalVariable* localVariables;
    struct Stack* operandStack;
    struct Machine* machine;
    struct ClassFile* classRef;
    uint32_t pc; // program counter
    Code_attribute* code;
    //A run-time constant pool is a per-class or per-interface run-time representation of the constant_pool table in a class file (§4.4).
    //It contains several kinds of constants, ranging from numeric literals known at compile-time to method and field references that must be resolved at run-time.
} Frame;

Frame createNewFrame (Code_attribute code, ClassFile* cf, Machine* machine);
void destroyFrame (Frame* frame);
#endif