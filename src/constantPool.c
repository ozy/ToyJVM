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
            fread(&info.info.class_info.name_index,sizeof(info.info.class_info.name_index),1,fd);
            info.info.class_info.name_index = be16toh(info.info.class_info.name_index); // big order to host
            break;
        case CONSTANT_Fieldref:
        case CONSTANT_Methodref:
        case CONSTANT_InterfaceMethodref:
            fread(&info.info.ref_info.class_index,sizeof(info.info.ref_info.class_index),1,fd);
            fread(&info.info.ref_info.name_and_type_index,sizeof(info.info.ref_info.name_and_type_index),1,fd);
            info.info.ref_info.class_index = be16toh(info.info.ref_info.class_index);
            info.info.ref_info.name_and_type_index = be16toh(info.info.ref_info.name_and_type_index);
            break;
        case CONSTANT_String:
            fread(&info.info.string_info.string_index,sizeof(info.info.string_info.string_index),1,fd);
            info.info.string_info.string_index = be16toh(info.info.string_info.string_index);
            break;
        case CONSTANT_Integer:
        case CONSTANT_Float:
            fread(&info.info._4BYTES_info.bytes,sizeof(info.info._4BYTES_info.bytes),1,fd);
            info.info._4BYTES_info.bytes = be32toh(info.info._4BYTES_info.bytes);
            break;
        case CONSTANT_Long:
        case CONSTANT_Double:
            // 8 byte types takes two indexes in constant pool. So increase the counter by one extra.
            fread(&info.info._8BYTES_info.bytes,sizeof(info.info._8BYTES_info.bytes),1,fd);
            info.info._8BYTES_info.bytes = be64toh(info.info._8BYTES_info.bytes);
            break;
        case CONSTANT_NameAndType:
            fread(&info.info.nameAndType_info.name_index,sizeof(info.info.nameAndType_info.name_index),1,fd);
            fread(&info.info.nameAndType_info.descriptor_index,sizeof(info.info.nameAndType_info.descriptor_index),1,fd);
            info.info.nameAndType_info.name_index = be16toh(info.info.nameAndType_info.name_index);
            info.info.nameAndType_info.descriptor_index = be16toh(info.info.nameAndType_info.descriptor_index);
            break;
        case CONSTANT_Utf8:; // compiler wants a statement after label.
            fread(&info.info.utf8_info.length,sizeof(info.info.utf8_info.length),1,fd); // len
            info.info.utf8_info.length = be16toh(info.info.utf8_info.length);
            info.info.utf8_info.bytes = malloc(info.info.utf8_info.length); //leak
            fread(info.info.utf8_info.bytes,info.info.utf8_info.length,1,fd); // offsetting 2 bytes + reading utf8
            //printf("len: %d, s: %.*s\n",info.info.utf8_info.length,info.info.utf8_info.length, info.info.utf8_info.bytes);
            break;
        case CONSTANT_MethodHandle:; // compiler wants a statement after label.
            fread(&info.info.methodHandle_info.reference_kind,sizeof(info.info.methodHandle_info.reference_kind),1,fd); // reference kind
            fread(&info.info.methodHandle_info.reference_index,sizeof(info.info.methodHandle_info.reference_index),1,fd); // ref index
            info.info.methodHandle_info.reference_index = be16toh(info.info.methodHandle_info.reference_index);
            break;
        case CONSTANT_MethodType:
            fread(&info.info.methodType_info.descriptor_index,sizeof(info.info.methodType_info.descriptor_index),1,fd);
            info.info.methodType_info.descriptor_index = be16toh(info.info.methodType_info.descriptor_index);
            break;
        case CONSTANT_InvokeDynamic:
            fread(&info.info.invodeDynamic_info.bootstrap_method_attr_index,sizeof(info.info.invodeDynamic_info.bootstrap_method_attr_index),1,fd);
            fread(&info.info.invodeDynamic_info.name_and_type_index,sizeof(info.info.invodeDynamic_info.name_and_type_index),1,fd);
            info.info.invodeDynamic_info.bootstrap_method_attr_index = be16toh(info.info.invodeDynamic_info.bootstrap_method_attr_index);
            info.info.invodeDynamic_info.name_and_type_index = be16toh(info.info.invodeDynamic_info.name_and_type_index);
            break;

        default:
            //tag err
            break;
    }
    return info;

}

void destroyCp_Info(cp_info* info){
    if (info->tag == CONSTANT_Utf8)
        free(info->info.utf8_info.bytes);
}