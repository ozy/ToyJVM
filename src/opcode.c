#include "opcode.h"
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include <endian.h>
#include "constantPool.h"
#include "frame.h"
#include "classFile.h"

uint8_t read1Byte(Frame* frame){
    return *(uint8_t*)&frame->code->code[++frame->pc]; // to get the first arg. of the opcode
}
uint16_t read2Bytes(Frame* frame){
    /*
    OP  A1  A2
    00  01  02
    ^
    */
    uint16_t val=be16toh(*(uint16_t*)&frame->code->code[frame->pc+1]);
    frame->pc += 2;
    return val;
}
uint32_t read4Bytes(Frame* frame){
    uint32_t val=be32toh(*(uint32_t*)&frame->code->code[frame->pc+1]);
    frame->pc += 4;
    return val;
}
uint64_t read8Bytes(Frame* frame){
    uint64_t val=be64toh(*(uint64_t*)&frame->code->code[frame->pc+1]);
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
void* OPCODE_DCONST_2(Frame* frame){
    double val = 2.0;
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
    cp_info aConstant = frame->classRef->constant_pool[index-1];
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
    cp_info aConstant = frame->classRef->constant_pool[index-1];
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
    //popStack(frame->operandStack);
    int64_t discardedRetVal = *(int64_t*) popStack(frame->operandStack);
    printf("Discarded Return Value is %d\n",discardedRetVal);
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

void* OPCODE_IF_ICMPGE(Frame* frame){
    uint16_t offset = read2Bytes(frame);
    int64_t val2 = (int32_t)*(int64_t*) popStack(frame->operandStack);
    int64_t val1 = (int32_t)*(int64_t*) popStack(frame->operandStack);
    if (val1 >= val2){
        frame->pc += offset - 3;
        // -1 because the main loop will increase 1 after this opcode to
        // get the next opcode
    }
	return NULL;
}

void* OPCODE_IINC(Frame* frame){
    uint8_t index = read1Byte(frame);
    uint8_t incrementAmount = read1Byte(frame);
    *(int64_t*)&frame->localVariables[index] = (int32_t)(*(int64_t*)&frame->localVariables[index] + *(int8_t*)&incrementAmount);
    return NULL;
}

void* OPCODE_GOTO(Frame* frame){
    uint16_t offset = read2Bytes(frame); // branch offset
    frame->pc += *(int16_t*)&offset - 3;
    // -1 because the main loop will increase 1 after this opcode to
    // get the next opcode
    return NULL;
}

void* OPCODE_IRETURN(Frame* frame){
    uint64_t* val = malloc(sizeof(val));
    *val = *(uint64_t*)popStack(frame->operandStack);
    return val;
}
void* OPCODE_RETURN(Frame* frame){
    popStack(frame->JVMSTACK);
    // current frame is discarded
    return NULL;
}
void* OPCODE_INVOKESTATIC(Frame* frame){
    uint16_t index = read2Bytes(frame); // normally indexes pointing into 
    // the constant pool are index - 1 in reality. But in our case, the
    // constant pool and the run-time constant pool are the same except
    // run time constant pool takes indexes as is but the normal constant
    // pool takes indexes starting from 1. The extra +1 is because
    Frame newFrame;

    if (!(frame->classRef->constant_pool[index-1].tag == CONSTANT_Methodref || frame->classRef->constant_pool[index-1].tag == CONSTANT_InterfaceMethodref)){
        // index must be a symbolic reference to a method or an interface method
        // if the resolved method is an instance method, the invokestatic instruction throws an IncompatibleClassChangeError. 
        printf("invoked method ref is invalid index is %d\n",index);
    }
    CONSTANT_Ref_info methodOrInterfaceRef = frame->classRef->constant_pool[index-1].info.ref_info;
    //printf ("Method's Class Name: %.*s\n",name_utf8.length,name_utf8.bytes);
    method_info* method = canClassHandleMethod(frame->classRef, methodOrInterfaceRef);

    if (method != NULL){
        newFrame.code = malloc (sizeof(Code_attribute));
        *newFrame.code = getCode_AttributeFromAttribute_info(method->attributes[0]);
        //method->attributes[0]
    }else{
        // todo method not found
        printf("method not found \n");
    }
    //CONSTANT_Utf8_info utf8 = frame->classRef->constant_pool[method->name_index-1].info.utf8_info;
    //printf ("Code Name: %.*s\n",utf8.length,utf8.bytes);
    //printf("max_stack : %d\n",attribute->max_stack);
    //printf("max_locals : %d\n",attribute->max_locals);
    //printf("code_lenght : %d\n",attribute->code_length);

    newFrame.localVariables = malloc (sizeof(LocalVariable) * newFrame.code->max_locals);
    if (method->access_flags != M_ACC_NATIVE){
        // On successful resolution of the method, the class or interface that declared the resolved method is initialized (§5.5) if that class or interface has not already been initialized. 
        int numArgs = getNumArgs(frame->classRef, methodOrInterfaceRef);
        for (int argId = numArgs-1; argId >= 0; argId--){
            uint64_t arg = *(uint64_t*) popStack(frame->operandStack);
            newFrame.localVariables[argId] = arg;
        }

    }

    newFrame.operandStack = malloc (sizeof(Stack));
    *newFrame.operandStack = initStack(newFrame.code->max_stack, TYPE_OPERANDSTACK);

    newFrame.pc = 0;
    newFrame.classRef = frame->classRef;
    newFrame.JVMSTACK = frame->JVMSTACK;

    pushStack(&newFrame, frame->JVMSTACK);

    frame->pc++;
    return NULL;
}
OPCODE** initOpcodes(){
    // jumptable
    OPCODE** opcodes = malloc (sizeof(OPCODE*) * 255); // leak
    opcodes[0x10] = OPCODE_BIPUSH;
    opcodes[0x12] = OPCODE_LDC;

    opcodes[0x1a] = OPCODE_NLOAD_0;
    opcodes[0x1b] = OPCODE_NLOAD_1;
    opcodes[0x1c] = OPCODE_NLOAD_2;
    opcodes[0x1d] = OPCODE_NLOAD_3;

    opcodes[0x2] = OPCODE_ICONST_M1;
    opcodes[0x3] = OPCODE_ICONST_0;

    opcodes[0x36] = OPCODE_NSTORE;

    opcodes[0x3b] = OPCODE_NSTORE_0;
    opcodes[0x3c] = OPCODE_NSTORE_1;
    opcodes[0x3d] = OPCODE_NSTORE_2;
    opcodes[0x3e] = OPCODE_NSTORE_3;

    opcodes[0x4] = OPCODE_ICONST_1;
    opcodes[0x5] = OPCODE_ICONST_2;

    opcodes[0x57] = OPCODE_POP;

    opcodes[0x6] = OPCODE_ICONST_3;
    opcodes[0x7] = OPCODE_ICONST_4;
    opcodes[0x8] = OPCODE_ICONST_5;

    opcodes[0x84] = OPCODE_IINC;

    opcodes[0x2a] = OPCODE_NLOAD_0;
    opcodes[0x2b] = OPCODE_NLOAD_1;

    opcodes[0x9] = OPCODE_ICONST_0;
    opcodes[0xa] = OPCODE_ICONST_1;

    opcodes[0xa2] = OPCODE_IF_ICMPGE;
    opcodes[0xa7] = OPCODE_GOTO;
    

    opcodes[0xb] = OPCODE_DCONST_0;
    opcodes[0xc] = OPCODE_DCONST_1;
    opcodes[0xd] = OPCODE_DCONST_2;

    opcodes[0xb1] = OPCODE_RETURN;
    opcodes[0xb7] = OPCODE_INVOKESTATIC;
    opcodes[0xb8] = OPCODE_INVOKESTATIC;
    
    opcodes[0x60] = OPCODE_IADD;
    opcodes[0xac] = OPCODE_IRETURN;
    return opcodes;
}