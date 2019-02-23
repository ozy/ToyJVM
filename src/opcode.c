#include "opcode.h"
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include <endian.h>
#include "constantPool.h"

OPCODE** initOpcodes(){
    // jumptable
    OPCODE** opcodes = malloc (sizeof(OPCODE*) * 255); // leak
    //(*opcodes)(int)[0] = &printSomething;
    //opcodes[0] = printSomething;
    return opcodes;
}
uint8_t read1Byte(Frame* frame){
    return *(uint8_t*)&frame->code.code[++frame->pc]; // to get the first arg. of the opcode
}
uint16_t read2Bytes(Frame* frame){
    /*
    OP  A1  A2
    00  01  02
    ^
    */
    uint16_t val=be16toh(*(uint16_t*)&frame->code.code[frame->pc+1]);
    frame->pc += 2;
    return val;
}
uint32_t read4Bytes(Frame* frame){
    uint32_t val=be32toh(*(uint32_t*)&frame->code.code[frame->pc+1]);
    frame->pc += 4;
    return val;
}
uint64_t read8Bytes(Frame* frame){
    uint64_t val=be64toh(*(uint64_t*)&frame->code.code[frame->pc+1]);
    frame->pc += 8;
    return val;
}
void OPCODE_ICONST_M1(Frame* frame){
    uint64_t val = -1;
    pushStack(&val,frame->operandStack);
}
void OPCODE_ICONST_0(Frame* frame){
    uint64_t val = 0;
    pushStack(&val,frame->operandStack);
}
void OPCODE_ICONST_1(Frame* frame){
    uint64_t val = 1;
    pushStack(&val,frame->operandStack);
}
void OPCODE_ICONST_2(Frame* frame){
    uint64_t val = 2;
    pushStack(&val,frame->operandStack);
}
void OPCODE_ICONST_3(Frame* frame){
    uint64_t val = 3;
    pushStack(&val,frame->operandStack);
}
void OPCODE_ICONST_4(Frame* frame){
    uint64_t val = 4;
    pushStack(&val,frame->operandStack);
}
void OPCODE_ICONST_5(Frame* frame){
    uint64_t val = 5;
    pushStack(&val,frame->operandStack);
}
void OPCODE_DCONST_0(Frame* frame){
    double val = 0.0;
    pushStack(&val,frame->operandStack);
}
void OPCODE_DCONST_1(Frame* frame){
    double val = 1.0;
    pushStack(&val,frame->operandStack);
}
void OPCODE_BIPUSH(Frame* frame){
    uint64_t val = read1Byte(frame);
    pushStack(&val,frame->operandStack);
}
void OPCODE_SIPUSH(Frame* frame){
    uint64_t val = read2Bytes(frame);
    pushStack(&val,frame->operandStack);
}
void OPCODE_LCD(Frame* frame){
    uint8_t index = read1Byte(frame);
    cp_info aConstant = frame->runTimeConstantPoolRef[index-1];
    if (aConstant.tag == CONSTANT_Integer || aConstant.tag == CONSTANT_Float){
        uint64_t val = *(uint32_t*)aConstant.info;
        pushStack(&val, frame->operandStack);
    }else{
        /*
        Otherwise, if the run-time constant pool entry is a reference to an instance of class 
        String representing a string literal (§5.1), then a reference to that instance, value,
        is pushed onto the operand stack.
        
        Otherwise, if the run-time constant pool entry is a symbolic reference to a class (§5.1),
        then the named class is resolved (§5.4.3.1) and a reference to the Class object 
        representing that class, value, is pushed onto the operand stack.

        Otherwise, the run-time constant pool entry must be a symbolic reference to a method type
        or a method handle (§5.1). The method type or method handle is resolved (§5.4.3.5) and a
        reference to the resulting instance of java.lang.invoke.MethodType or 
        java.lang.invoke.MethodHandle, value, is pushed onto the operand stack. 
        */
    }
}