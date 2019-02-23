#include "constantPool.h"
#include <stdio.h>
#include <stdlib.h>
/*
Constant Pool Indexes written in the class file starts from 1; 
to access a constant, please get the (index - 1)th value.
*/
cp_info cp_infoFromFile(FILE* fd){
    cp_info info;
    fread(&info.tag,1,1,fd); // tag len is 1
    
    switch (info.tag){
        case CONSTANT_Class:
            info.info = malloc (sizeof(CONSTANT_Class_info)); // leak
            fread(info.info,sizeof(CONSTANT_Class_info),1,fd);
            ((CONSTANT_Class_info*)info.info)->name_index = be16toh(((CONSTANT_Class_info*)info.info)->name_index); // big order to host
            // directly assigning memory saves a lot of memory access and
            // moving operations. (Ofc for non padded ones.)
            break;
        case CONSTANT_Fieldref: 
        case CONSTANT_Methodref:
        case CONSTANT_InterfaceMethodref:
            info.info = malloc (sizeof(CONSTANT_Methodref_info)); // leak
            fread(info.info,sizeof(CONSTANT_Methodref_info),1,fd);
            ((CONSTANT_Methodref_info*)info.info)->class_index = be16toh(((CONSTANT_Methodref_info*)info.info)->class_index); // big order to host
            ((CONSTANT_Methodref_info*)info.info)->name_and_type_index = be16toh(((CONSTANT_Methodref_info*)info.info)->name_and_type_index); // big order to host
            break;
        case CONSTANT_String:
            info.info = malloc (sizeof(CONSTANT_String_info)); // leak
            fread(info.info,sizeof(CONSTANT_String_info),1,fd);
            ((CONSTANT_String_info*)info.info)->string_index = be16toh(((CONSTANT_String_info*)info.info)->string_index); // big order to host
            break;
        case CONSTANT_Integer:
        case CONSTANT_Float:
            info.info = malloc (sizeof(CONSTANT_Integer_info)); // leak
            fread(info.info,sizeof(CONSTANT_Integer_info),1,fd);
            ((CONSTANT_Integer_info*)info.info)->bytes = be32toh(((CONSTANT_Integer_info*)info.info)->bytes); // big order to host
            break;
        case CONSTANT_Long:
        case CONSTANT_Double:
            info.info = malloc (sizeof(CONSTANT_Long_info)); // leak
            fread(info.info,sizeof(CONSTANT_Long_info),1,fd);
            ((CONSTANT_Long_info*)info.info)->bytes = be64toh(((CONSTANT_Long_info*)info.info)->bytes); // big order to host
            break;
        case CONSTANT_NameAndType:
            info.info = malloc (sizeof(CONSTANT_NameAndType_info)); // leak
            fread(info.info,sizeof(CONSTANT_NameAndType_info),1,fd);
            ((CONSTANT_NameAndType_info*)info.info)->name_index = be16toh(((CONSTANT_NameAndType_info*)info.info)->name_index); // big order to host
            ((CONSTANT_NameAndType_info*)info.info)->descriptor_index = be16toh(((CONSTANT_NameAndType_info*)info.info)->descriptor_index); // big order to host
            break;
        case CONSTANT_Utf8:; // compiler wants a statement after label.
            uint16_t length;
            fread(&length,sizeof(length),1,fd); // len
            length = be16toh(length);
            info.info = malloc(sizeof(length) + length); //leak
            fread(info.info + sizeof(length),length,1,fd); // offsetting 2 bytes + reading utf8
            break;
        case CONSTANT_MethodHandle:; // compiler wants a statement after label.
            CONSTANT_MethodHandle_info mhInfo;
            info.info = malloc (3);
            fread(&mhInfo.reference_kind,sizeof(mhInfo.reference_kind),1,fd); // reference kind
            fread(&mhInfo.reference_index,sizeof(mhInfo.reference_index),1,fd); // ref index
            *(uint8_t*)info.info = mhInfo.reference_kind;
            *(uint16_t*)(info.info+1) = be16toh(mhInfo.reference_index);
            break;
        case CONSTANT_MethodType:
            info.info = malloc (sizeof(CONSTANT_MethodType_info)); // leak
            fread(info.info,sizeof(CONSTANT_MethodType_info),1,fd);
            ((CONSTANT_MethodType_info*)info.info)->descriptor_index = be16toh(((CONSTANT_MethodType_info*)info.info)->descriptor_index); // big order to host

            break;
        case CONSTANT_InvokeDynamic:
            info.info = malloc (sizeof(CONSTANT_InvokeDynamic_info)); // leak
            fread(info.info,sizeof(CONSTANT_InvokeDynamic_info),1,fd);
            ((CONSTANT_InvokeDynamic_info*)info.info)->bootstrap_method_attr_index = be16toh(((CONSTANT_InvokeDynamic_info*)info.info)->bootstrap_method_attr_index); // big order to host
            ((CONSTANT_InvokeDynamic_info*)info.info)->name_and_type_index = be16toh(((CONSTANT_InvokeDynamic_info*)info.info)->name_and_type_index); // big order to host
            break;

        default:
            //tag err
            break;
    }
    return info;

}