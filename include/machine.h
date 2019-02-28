#ifndef MACHINE_H_
#define MACHINE_H_
#include "opcode.h"
#include "stack.h"

void* executeCode(Stack* JVMSTACK, OPCODE** opcodes);

typedef struct Machine{
    struct ClassFile* classFiles;
    uint64_t numClasses;
}Machine;


#endif