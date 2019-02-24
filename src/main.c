#include <stdio.h>
#include <stdlib.h>
//#include "heap.h"
#include "classFile.h"
#include <assert.h>
#include "opcode.h" // gecici
#include "stack.h"
#include "attribute.h"
#include "constantPool.h"

int main(){
    //(*initOpcodes()[0])(1, 1);
    constant* constantPool = malloc (sizeof(constant)*255);

    Stack JVMSTACK = initStack(255,TYPE_JVMSTACK);

    ClassFile myClass = fromClassFile("test/ArrayTest.class");
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
        CONSTANT_Utf8_info utf8;
        utf8.length = *(uint16_t*) myClass.constant_pool[myClass.methods[i].name_index].info;
        utf8.bytes = myClass.constant_pool[myClass.methods[i].name_index].info + 2;
        printf ("Method Name: %-*s\n",utf8.length,utf8.bytes);
        printf("-\n");
        for (int a = 0; a < myClass.methods[i].attributes_count; a++){
            if (myClass.methods[i].access_flags == M_ACC_NATIVE || myClass.methods[i].access_flags == M_ACC_ABSTRACT){
                attribute_info attribute;
                attribute = myClass.methods[i].attributes[a];
                utf8.length = *(uint16_t*) myClass.constant_pool[attribute.attribute_name_index].info;
                utf8.bytes = myClass.constant_pool[attribute.attribute_name_index].info + 2;
                printf ("Attribute Name: %-*s\n",utf8.length,utf8.bytes);
            }else{
                Code_attribute attribute;
                attribute = getCode_AttributeFromAttribute_info(myClass.methods[i].attributes[a]);
                printf("max_stack : %d\n",attribute.max_stack);
                printf("max_locals : %d\n",attribute.max_locals);
                printf("code_lenght : %d\n",attribute.code_length);
            }
        }
        printf("----------\n");
    }

    cp_info info = myClass.constant_pool[myClass.this_class];
    CONSTANT_Class_info* classInfo = (CONSTANT_Class_info*)info.info;

    if (info.tag == CONSTANT_Class){
        CONSTANT_Utf8_info utf8;
        utf8.length = *(uint16_t*) myClass.constant_pool[classInfo->name_index].info;
        utf8.bytes = myClass.constant_pool[classInfo->name_index].info + 2;

        printf ("This Class Name: %-*s\n",utf8.length,utf8.bytes);

    }

    for (int i = 0; i<myClass.constant_pool_count; i++){
        cp_info info = myClass.constant_pool[i];
        CONSTANT_Class_info* classInfo = (CONSTANT_Class_info*)info.info;

        if (info.tag == CONSTANT_Class){
            CONSTANT_Utf8_info utf8;
            utf8.length = *(uint16_t*) myClass.constant_pool[classInfo->name_index].info;
            utf8.bytes = myClass.constant_pool[classInfo->name_index].info + 2;

            printf ("Class Name: %-*s\n",utf8.length,utf8.bytes);

        }

    }



}