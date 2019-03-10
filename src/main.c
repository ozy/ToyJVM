#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
#include "classFile.h"
#include <assert.h>
#include "opcode.h" // gecici
#include "stack.h"
#include "attribute.h"
#include "constantPool.h"
#include "machine.h"
#include "heap.h"
#include "debug.h"

void main(int argc, char* argv[]){
    Machine JVM;
    JVM.numClasses = 0;
    JVM.classFiles = malloc(sizeof(ClassFile) * 255);
    ClassFile* mainClass = getClassFromName(argv[1], &JVM);

    assert(checkFormat(mainClass)); // file format check
    //DEBUG_PRINT(("%x\n",mainClass->magic));
    DEBUG_PRINT(("Minor: %d, Major: %d\n", mainClass->minor_version,mainClass->major_version));
    DEBUG_PRINT(("constant_pool_count: %d\n",mainClass->constant_pool_count));
    DEBUG_PRINT(("access_flags: %d\n",mainClass->access_flags));
    DEBUG_PRINT(("this_class: %d\n",mainClass->this_class));
    DEBUG_PRINT(("super_class: %d\n",mainClass->super_class));
    DEBUG_PRINT(("interfaces_count: %d\n",mainClass->interfaces_count));
    DEBUG_PRINT(("fields_count: %d\n",mainClass->fields_count));
    DEBUG_PRINT(("methods_count: %d\n",mainClass->methods_count));
    DEBUG_PRINT(("attributes_count: %d\n",mainClass->attributes_count));

    cp_info info = mainClass->constant_pool[mainClass->this_class-1];
    CONSTANT_Class_info classInfo = info.info.class_info;

    if (info.tag == CONSTANT_Class){
        CONSTANT_Utf8_info utf8 = mainClass->constant_pool[classInfo.name_index-1].info.utf8_info;
        DEBUG_PRINT( ("This Class Name: %.*s\n",utf8.length,utf8.bytes));
    }

    JVM.heap = malloc(sizeof(Heap));
    *JVM.heap = initHeap(512);

    JVM.JVMSTACK = malloc(sizeof(Stack));
    *JVM.JVMSTACK = initStack(255,TYPE_JVMSTACK);

    Frame newFrame;

    method_info* method = getMethodByName(mainClass, "main","([Ljava/lang/String;)V");
    Code_attribute* code = malloc(sizeof(Code_attribute));
    if (method != NULL){
        *code = getCode_AttributeFromAttribute_info(method->attributes[0]);
        //method->attributes[0]
    }else{
        printf("main method not found \n");
        return;
    }

    newFrame.code = code;
    newFrame.localVariables = malloc (sizeof(LocalVariable) * code->max_locals);

    //Stack* operandStack = ;
    newFrame.operandStack = malloc(sizeof(Stack));
    *newFrame.operandStack = initStack(code->max_stack, TYPE_OPERANDSTACK);

    newFrame.pc = 0;
    newFrame.classRef = mainClass;
    newFrame.machine = &JVM;

    pushStack(&newFrame, JVM.JVMSTACK);
    void* retCode = executeCode(&JVM, initOpcodes());
    if (retCode == NULL){
        DEBUG_PRINT(("Execution Stack Finished\n"));
    }
    destroyMachine(&JVM);
}