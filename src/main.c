#include <stdio.h>
#include <stdlib.h>
//#include "heap.h"
#include "machine.h"
#include "classFile.h"
#include <assert.h>
#include "opcode.h" // gecici
#include "stack.h"
#include "attribute.h"
#include "constantPool.h"
#include "machine.h"

void main(int argc, char* argv[]){
    Machine JVM;
    JVM.numClasses = 0;
    JVM.classFiles = malloc(sizeof(ClassFile) * 255);
    ClassFile* mainClass = getClassFromName(argv[1], &JVM);

    assert(checkFormat(mainClass)); // file format check
    //printf("%x\n",mainClass->magic);
    printf("Minor: %d, Major: %d\n", mainClass->minor_version,mainClass->major_version);
    printf("constant_pool_count: %d\n",mainClass->constant_pool_count);
    printf("access_flags: %d\n",mainClass->access_flags);
    printf("this_class: %d\n",mainClass->this_class);
    printf("super_class: %d\n",mainClass->super_class);
    printf("interfaces_count: %d\n",mainClass->interfaces_count);
    printf("fields_count: %d\n",mainClass->fields_count);
    printf("methods_count: %d\n",mainClass->methods_count);
    printf("attributes_count: %d\n",mainClass->attributes_count);

    for (int i = 0; i<mainClass->methods_count; i++){
        printf("index: %d\n",i+1); // constant pool starts from 1
        CONSTANT_Utf8_info utf8 = mainClass->constant_pool[mainClass->methods[i].name_index-1].info.utf8_info;
        CONSTANT_Utf8_info desc = mainClass->constant_pool[mainClass->methods[i].descriptor_index-1].info.utf8_info;
        printf ("Method Name: %.*s, %.*s\n", utf8.length, utf8.bytes, desc.length, desc.bytes);
        printf("-\n");
        for (int a = 0; a < mainClass->methods[i].attributes_count; a++){
            if (mainClass->methods[i].access_flags == M_ACC_NATIVE || mainClass->methods[i].access_flags == M_ACC_ABSTRACT){
                attribute_info attribute;
                attribute = mainClass->methods[i].attributes[a];
                utf8 = mainClass->constant_pool[attribute.attribute_name_index-1].info.utf8_info;
                printf ("Attribute Name: %.*s\n",utf8.length,utf8.bytes);
            }else{
                Code_attribute attribute;
                attribute = getCode_AttributeFromAttribute_info(mainClass->methods[i].attributes[a]);
                utf8 = mainClass->constant_pool[attribute.attribute_name_index-1].info.utf8_info;
                printf ("Code Name: %.*s\n",utf8.length,utf8.bytes);
                printf("max_stack : %d\n",attribute.max_stack);
                printf("max_locals : %d\n",attribute.max_locals);
                printf("code_lenght : %d\n",attribute.code_length);
            }
        }
        printf("----------\n");
    }

    cp_info info = mainClass->constant_pool[mainClass->this_class-1];
    CONSTANT_Class_info classInfo = info.info.class_info;

    if (info.tag == CONSTANT_Class){
        CONSTANT_Utf8_info utf8 = mainClass->constant_pool[classInfo.name_index-1].info.utf8_info;
        printf ("This Class Name: %.*s\n",utf8.length,utf8.bytes);
    }

    for (int i = 0; i<mainClass->constant_pool_count-1; i++){
        cp_info info = mainClass->constant_pool[i];

        if (info.tag == CONSTANT_Class){
            CONSTANT_Class_info classInfo = info.info.class_info;
            CONSTANT_Utf8_info utf8 = mainClass->constant_pool[classInfo.name_index-1].info.utf8_info;
            printf ("Class Name: %.*s\n",utf8.length,utf8.bytes);

        }
        if (info.tag == CONSTANT_InterfaceMethodref || info.tag == CONSTANT_Methodref){
            CONSTANT_Ref_info methodOrInterfaceRef = info.info.ref_info;
            
            CONSTANT_NameAndType_info* nameAndType = &mainClass->constant_pool[methodOrInterfaceRef.name_and_type_index-1].info.nameAndType_info;
            CONSTANT_Utf8_info name_utf8 = mainClass->constant_pool[nameAndType->name_index-1].info.utf8_info;
            printf ("%d: Method's Name: %.*s\n",i,name_utf8.length,name_utf8.bytes);
        }

    }

    Stack JVMSTACK = initStack(255,TYPE_JVMSTACK);
    JVM.JVMSTACK = &JVMSTACK;
    Frame newFrame;

    method_info* method = getMethodByName(mainClass, "main","([Ljava/lang/String;)V");
    Code_attribute code;
    if (method != NULL){
        code = getCode_AttributeFromAttribute_info(method->attributes[0]);
        //method->attributes[0]
    }else{
        printf("main method not found \n");
        return;
    }

    newFrame.code = &code;
    newFrame.localVariables = malloc (sizeof(LocalVariable) * code.max_locals);

    Stack operandStack = initStack(code.max_stack, TYPE_OPERANDSTACK);
    newFrame.operandStack = &operandStack;
    
    newFrame.pc = 0;
    newFrame.classRef = mainClass;
    newFrame.machine = &JVM;

    pushStack(&newFrame, &JVMSTACK);
    void* retCode = executeCode(&JVMSTACK, initOpcodes());
    if (retCode == NULL){
        printf("Execution Stack Finished\n");
    }

}