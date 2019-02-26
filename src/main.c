#include <stdio.h>
#include <stdlib.h>
//#include "heap.h"
#include "classFile.h"
#include <assert.h>
#include "opcode.h" // gecici
#include "stack.h"
#include "attribute.h"
#include "constantPool.h"
#include "machine.h"

int main(){
    //(*initOpcodes()[0])(1, 1);
    cp_info* rt_constantPool = malloc (sizeof(cp_info)*255);

    Stack JVMSTACK = initStack(255,TYPE_JVMSTACK);

    ClassFile myClass = fromClassFile("test/returnTest.class");
    assert(checkFormat(myClass)); // file format check
    //printf("%x, checkStatus: %d\n",myClass.magic,checkFormat(myClass));
    printf("Minor: %d, Major: %d\n", myClass.minor_version,myClass.major_version);
    printf("constant_pool_count: %d\n",myClass.constant_pool_count);
    printf("access_flags: %d\n",myClass.access_flags);
    printf("this_class: %d\n",myClass.this_class);
    printf("super_class: %d\n",myClass.super_class);
    printf("interfaces_count: %d\n",myClass.interfaces_count);
    printf("fields_count: %d\n",myClass.fields_count);
    printf("methods_count: %d\n",myClass.methods_count);
    printf("attributes_count: %d\n",myClass.attributes_count);

    for (int i = 0; i<myClass.methods_count; i++){
        printf("index: %d\n",i+1); // constant pool starts from 1
        CONSTANT_Utf8_info utf8 = myClass.constant_pool[myClass.methods[i].name_index-1].info.utf8_info;
        printf ("Method Name: %d, %.*s\n",utf8.length,utf8.length,utf8.bytes);
        printf("-\n");
        for (int a = 0; a < myClass.methods[i].attributes_count; a++){
            if (myClass.methods[i].access_flags == M_ACC_NATIVE || myClass.methods[i].access_flags == M_ACC_ABSTRACT){
                attribute_info attribute;
                attribute = myClass.methods[i].attributes[a];
                utf8 = myClass.constant_pool[attribute.attribute_name_index-1].info.utf8_info;
                printf ("Attribute Name: %.*s\n",utf8.length,utf8.bytes);
            }else{
                Code_attribute attribute;
                attribute = getCode_AttributeFromAttribute_info(myClass.methods[i].attributes[a]);
                utf8 = myClass.constant_pool[attribute.attribute_name_index-1].info.utf8_info;
                printf ("Code Name: %.*s\n",utf8.length,utf8.bytes);
                printf("max_stack : %d\n",attribute.max_stack);
                printf("max_locals : %d\n",attribute.max_locals);
                printf("code_lenght : %d\n",attribute.code_length);
            }
        }
        printf("----------\n");
    }

    cp_info info = myClass.constant_pool[myClass.this_class-1];
    CONSTANT_Class_info classInfo = info.info.class_info;

    if (info.tag == CONSTANT_Class){
        CONSTANT_Utf8_info utf8 = myClass.constant_pool[classInfo.name_index-1].info.utf8_info;
        printf ("This Class Name: %.*s\n",utf8.length,utf8.bytes);
    }

    for (int i = 0; i<myClass.constant_pool_count-1; i++){
        cp_info info = myClass.constant_pool[i];
        CONSTANT_Class_info classInfo = info.info.class_info;

        if (info.tag == CONSTANT_Class){
            CONSTANT_Utf8_info utf8 = myClass.constant_pool[classInfo.name_index-1].info.utf8_info;
            printf ("Class Name: %.*s\n",utf8.length,utf8.bytes);

        }

    }

    Frame fakeFrame;
    fakeFrame.localVariables = malloc (sizeof(LocalVariable) * 2);
    fakeFrame.localVariables[0]= 30;
    fakeFrame.localVariables[1]= 11;
    

    Stack operandStack = initStack(2, TYPE_OPERANDSTACK);
    fakeFrame.operandStack = &operandStack;

    Code_attribute attribute;
    attribute = getCode_AttributeFromAttribute_info(myClass.methods[2].attributes[0]);
    fakeFrame.code = attribute;

    fakeFrame.pc = 0;

    fakeFrame.runTimeConstantPoolRef = malloc(sizeof(cp_info) * 255);

    void* retCode = executeCode(fakeFrame, initOpcodes());
    printf("return is %d \n", *(uint64_t*)retCode);

}