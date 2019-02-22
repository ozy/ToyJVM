#include <stdio.h>
#include <stdlib.h>
//#include "heap.h"
#include "classFile.h"

int main(){
    //Heap heap;
    //unsigned char* classArea;
    ClassFile myClass = fromClassFile("test/ArrayTest.class");
    printf("%x\n",myClass.magic);
    printf("Minor: %d, Major: %d\n", myClass.minor_version,myClass.major_version);
    printf("constant_pool_count: %d\n",myClass.constant_pool_count);
    printf("access_flags: %d\n",myClass.access_flags);
    printf("this_class: %d\n",myClass.this_class);
    printf("super_class: %d\n",myClass.super_class);
    printf("interfaces_count: %d\n",myClass.interfaces_count);
    printf("fields_count: %d\n",myClass.fields_count);
    printf("methods_count: %d\n",myClass.methods_count);
    printf("attributes_count: %d\n",myClass.attributes_count);

    for (int i = 0; i<myClass.constant_pool_count-1; i++){
        printf("index: %d\n",i+1); // constant pool starts from 1
        printf("Type: %d\n",myClass.constant_pool[i].tag);
        if (myClass.constant_pool[i].tag == CONSTANT_Utf8){
            //printf("val : %-*s\n", *(uint16_t*)myClass.constant_pool[i].info,myClass.constant_pool[i].info+2);
            
        }else if(myClass.constant_pool[i].tag == CONSTANT_Class){
            CONSTANT_Class_info* class;
            class = (CONSTANT_Class_info*)myClass.constant_pool[i].info;
            CONSTANT_Utf8_info utf8;
            utf8.length = *(uint16_t*) myClass.constant_pool[class->name_index].info;
            utf8.bytes = myClass.constant_pool[class->name_index].info + 2;
            printf ("ClassName: %-*s\n",utf8.length,utf8.bytes);
        }
        printf("-----\n");
    }

}