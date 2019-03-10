#include "attribute.h"
#include <stdlib.h>
#include <endian.h>
#include <string.h>
attribute_info getAttribute_Info(FILE* fd){
    attribute_info attribute;
    fread(&attribute.attribute_name_index,sizeof(attribute.attribute_name_index),1,fd);
    attribute.attribute_name_index = be16toh(attribute.attribute_name_index);

    fread(&attribute.attribute_length,sizeof(attribute.attribute_length),1,fd);
    attribute.attribute_length = be32toh(attribute.attribute_length);

    attribute.info = malloc(attribute.attribute_length); // leak
    fread(attribute.info,attribute.attribute_length,1,fd);
    return attribute;
}

void destroyAttribute_Info(attribute_info* attribute){
    free(attribute->info);
}

Code_attribute getCode_AttributeFromAttribute_info(attribute_info attributeInfo){
    Code_attribute attribute;
    /*
    uint16_t attribute_name_index;
    uint32_t attribute_length;
    uint16_t max_stack;
    uint16_t max_locals;
    uint32_t code_length;
    uint8_t* code;
    uint16_t exception_table_lenght;
    exception_table* exceptionTable;
    uint16_t attributes_count;
    attribute_info* attributes;
    */
    attribute.attribute_name_index = attributeInfo.attribute_name_index;
    attribute.attribute_length = attributeInfo.attribute_length;

    int memOffset=0;
    memcpy(&attribute.max_stack, attributeInfo.info+memOffset,sizeof(attribute.max_stack));
    attribute.max_stack = be16toh(attribute.max_stack);
    memOffset +=sizeof(attribute.max_stack);

    memcpy(&attribute.max_locals, attributeInfo.info+memOffset,sizeof(attribute.max_locals));
    attribute.max_locals = be16toh(attribute.max_locals);
    memOffset +=sizeof(attribute.max_locals);

    memcpy(&attribute.code_length, attributeInfo.info+memOffset,sizeof(attribute.code_length));
    attribute.code_length = be32toh(attribute.code_length);
    memOffset +=sizeof(attribute.code_length);

    attribute.code = attributeInfo.info+memOffset; // leak
    memOffset +=attribute.code_length;

    // remainings will be implemented later. 
    return attribute;
}
