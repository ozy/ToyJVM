#include "machine.h"
#include "frame.h"
#include "opcode.h"
#include "stack.h"

void* executeCode(Frame frame, OPCODE** opcodes){ // anything can be returned from an execution
    for (int i =0; i<frame.code.code_length; i++){
        printf("0x%1x, opStackTop: %d\n", frame.code.code[i],frame.operandStack->top);
        void* retCode = opcodes[frame.code.code[i]](&frame);
        if (retCode != NULL){
            return (retCode);
        }
    }
    return NULL;
}