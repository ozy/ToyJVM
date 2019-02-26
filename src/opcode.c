#include "opcode.h"
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include <endian.h>
#include "constantPool.h"

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
void* OPCODE_ICONST_M1(Frame* frame){
    uint64_t val = -1;
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_ICONST_0(Frame* frame){
    uint64_t val = 0;
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_ICONST_1(Frame* frame){
    uint64_t val = 1;
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_ICONST_2(Frame* frame){
    uint64_t val = 2;
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_ICONST_3(Frame* frame){
    uint64_t val = 3;
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_ICONST_4(Frame* frame){
    uint64_t val = 4;
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_ICONST_5(Frame* frame){
    uint64_t val = 5;
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_DCONST_0(Frame* frame){
    double val = 0.0;
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_DCONST_1(Frame* frame){
    double val = 1.0;
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_BIPUSH(Frame* frame){
    uint64_t val = read1Byte(frame);
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_SIPUSH(Frame* frame){
    uint64_t val = read2Bytes(frame);
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_LDC(Frame* frame){
    uint8_t index = read1Byte(frame);
    cp_info aConstant = frame->runTimeConstantPoolRef[index-1];
    uint64_t val;
    if (aConstant.tag == CONSTANT_Integer){
        val = aConstant.info._4BYTES_info.bytes;
        pushStack(&val, frame->operandStack);
    }else if (aConstant.tag == CONSTANT_Float){
        val = aConstant.info._4BYTES_info.bytes;
        pushStack(&val, frame->operandStack);
    }else{
        val = (uint64_t)&aConstant.info;
        pushStack(&val, frame->operandStack);
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
    return NULL;
}
void* OPCODE_LCD_W(Frame* frame){
    uint16_t index = read2Bytes(frame);
    cp_info aConstant = frame->runTimeConstantPoolRef[index-1];
    uint64_t val;
    if (aConstant.tag == CONSTANT_Integer){
        val = aConstant.info._4BYTES_info.bytes;
        pushStack(&val, frame->operandStack);
    }else if (aConstant.tag == CONSTANT_Float){
        val = aConstant.info._4BYTES_info.bytes;
        pushStack(&val, frame->operandStack);
    }else{
        val = (uint64_t)&aConstant.info;
        pushStack(&val, frame->operandStack);
    }
	return NULL;
}
void* OPCODE_NLOAD(Frame* frame){ // N represents I,L or D
    uint8_t index = read1Byte(frame);
    pushStack(&frame->localVariables[index], frame->operandStack);
	return NULL;
}
void* OPCODE_NLOAD_0(Frame* frame){
    pushStack(&frame->localVariables[0], frame->operandStack);
	return NULL;
}
void* OPCODE_NLOAD_1(Frame* frame){
    pushStack(&frame->localVariables[1], frame->operandStack);
	return NULL;
}
void* OPCODE_NLOAD_2(Frame* frame){
    pushStack(&frame->localVariables[2], frame->operandStack);
	return NULL;
}
void* OPCODE_NLOAD_3(Frame* frame){
    pushStack(&frame->localVariables[3], frame->operandStack);
	return NULL;
}
void* OPCODE_NSTORE(Frame* frame){
    uint8_t index = read1Byte(frame);
    uint64_t val = *(uint64_t*)popStack(frame->operandStack);
    frame->localVariables[index] = val;
	return NULL;
}
void* OPCODE_NSTORE_0(Frame* frame){
    uint64_t val = *(uint64_t*)popStack(frame->operandStack);
    frame->localVariables[0] = val;
	return NULL;
}
void* OPCODE_NSTORE_1(Frame* frame){
    uint64_t val = *(uint64_t*)popStack(frame->operandStack);
    frame->localVariables[1] = val;
	return NULL;
}
void* OPCODE_NSTORE_2(Frame* frame){
    uint64_t val = *(uint64_t*)popStack(frame->operandStack);
    frame->localVariables[2] = val;
	return NULL;
}
void* OPCODE_NSTORE_3(Frame* frame){
    uint64_t val = *(uint64_t*)popStack(frame->operandStack);
    frame->localVariables[3] = val;
	return NULL;
}
void* OPCODE_POP(Frame* frame){
    popStack(frame->operandStack);
	return NULL;
}
void* OPCODE_DUP(Frame* frame){
    uint64_t val = *(uint64_t*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}
/////////////
// INTEGER //
/////////////
void* OPCODE_IADD(Frame* frame){
    int64_t val = (int32_t)*(int64_t*)popStack(frame->operandStack) + (int32_t)*(int64_t*)popStack(frame->operandStack); // java has no unsigned types, lol
    // the stacks are 64 bit but the int size is 32 bit
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_ISUB(Frame* frame){
    int32_t val2 = *(int64_t*) popStack(frame->operandStack);
    int32_t val1 = *(int64_t*) popStack(frame->operandStack);
    int64_t val = val1 - val2;
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_IMUL(Frame* frame){
    int64_t val = (int32_t)*(int64_t*)popStack(frame->operandStack) * (int32_t)*(int64_t*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_IDIV(Frame* frame){
    int32_t val2 = *(int64_t*) popStack(frame->operandStack);
    int32_t val1 = *(int64_t*) popStack(frame->operandStack);
    int64_t val = val1 / val2;
    pushStack(&val, frame->operandStack);
	return NULL;
}
/////////////
///  LONG ///
/////////////
void* OPCODE_LADD(Frame* frame){
    int64_t val = *(int64_t*)popStack(frame->operandStack) + *(int64_t*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_LSUB(Frame* frame){
    int64_t val2 = *(int64_t*) popStack(frame->operandStack);
    int64_t val1 = *(int64_t*) popStack(frame->operandStack);
    int64_t val = val1-val2;
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_LMUL(Frame* frame){
    int64_t val = *(int64_t*)popStack(frame->operandStack) * *(int64_t*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_LDIV(Frame* frame){
    int64_t val2 = *(int64_t*) popStack(frame->operandStack);
    int64_t val1 = *(int64_t*) popStack(frame->operandStack);
    int64_t val = val1 / val2;
    pushStack(&val, frame->operandStack);
	return NULL;
}
/////////////
/// DOUBLE //
/////////////
void* OPCODE_DADD(Frame* frame){
    double val = *(double*)popStack(frame->operandStack) + *(double*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_DSUB(Frame* frame){
    double val2 = *(double*) popStack(frame->operandStack);
    double val1 = *(double*) popStack(frame->operandStack);
    double val = val1-val2;
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_DMUL(Frame* frame){
    double val = *(double*)popStack(frame->operandStack) * *(double*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_DDIV(Frame* frame){
    double val2 = *(double*) popStack(frame->operandStack);
    double val1 = *(double*) popStack(frame->operandStack);
    double val = val1 / val2;
    pushStack(&val, frame->operandStack);
	return NULL;
}

///////////////

void* OPCODE_I2D(Frame* frame){
    double val = (double)(int32_t)*(int64_t*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}

void* OPCODE_I2F(Frame* frame){
    float val = (float)(int32_t)*(int64_t*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}

void* OPCODE_I2B(Frame* frame){
    int64_t val = (int8_t)*(int64_t*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}

void* OPCODE_I2C(Frame* frame){
    //char, whose values are 16-bit unsigned integers representing Unicode code points ...
    uint64_t val = (uint16_t)*(int64_t*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}

void* OPCODE_I2L(Frame* frame){
    uint64_t val = (int32_t)*(int64_t*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}

void* OPCODE_I2S(Frame* frame){
    uint64_t val = (int16_t)*(int64_t*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}

///////////////

void* OPCODE_DCMPG(Frame* frame){
    double val2 = *(double*) popStack(frame->operandStack);
    double val1 = *(double*) popStack(frame->operandStack);
    int64_t val = 0;
    if (val1 > val2){
        val = 1;
    }else if (val1 == val2){
        val = 0;
    }else {
        val = -1;
    }
    pushStack(&val, frame->operandStack);
	return NULL;
}

void* OPCODE_ILOAD_0(Frame* frame){
    pushStack(&frame->localVariables[0], frame->operandStack);
	return NULL;
}
void* OPCODE_ILOAD_1(Frame* frame){
    pushStack(&frame->localVariables[1], frame->operandStack);
	return NULL;
}
void* OPCODE_ILOAD_2(Frame* frame){
    pushStack(&frame->localVariables[2], frame->operandStack);
	return NULL;
}
void* OPCODE_ILOAD_3(Frame* frame){
    pushStack(&frame->localVariables[3], frame->operandStack);
	return NULL;
}

void* OPCODE_IRETURN(Frame* frame){
    uint64_t* val = malloc(sizeof(val));
    *val = *(uint64_t*)popStack(frame->operandStack);
    return val;
}

OPCODE** initOpcodes(){
    // jumptable
    OPCODE** opcodes = malloc (sizeof(OPCODE*) * 255); // leak
    //(*opcodes)(int)[0] = &printSomething;
    opcodes[0x1a] = OPCODE_ILOAD_0;
    opcodes[0x1b] = OPCODE_ILOAD_1;
    opcodes[0x60] = OPCODE_IADD;
    opcodes[0xac] = OPCODE_IRETURN;
    return opcodes;
}