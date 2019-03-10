#ifndef MACHINE_H_
#define MACHINE_H_
#include "opcode.h"
#include "stack.h"
#include "heap.h"

typedef struct Machine{
    struct ClassFile* classFiles;
    uint64_t numClasses;
    Stack* JVMSTACK;
    Heap* heap;
}Machine;

void* executeCode(Machine* machine, OPCODE** opcodes);
void destroyMachine(Machine* machine);

#endif