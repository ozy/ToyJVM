#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_
#include <inttypes.h>
#include <stdio.h>

typedef struct attribute_info {
    uint16_t attribute_name_index;
    uint32_t attribute_length;
    uint8_t* info;
}attribute_info;

typedef struct exception_table{ // aligned.
    uint16_t start_pc;
    uint16_t end_pc;
    uint16_t handler_pc;
    uint16_t catch_type;
} exception_table;

//If the method is either native or abstract, its method_info structure must not have a 
//Code attribute in its attributes table. Otherwise, its method_info structure must have
//exactly one Code attribute in its attributes table
typedef struct Code_attribute {
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
}Code_attribute;

attribute_info getAttribute_Info(FILE* fd);
Code_attribute getCode_AttributeFromAttribute_info(attribute_info fd);

void destroyAttribute_Info(attribute_info* attribute);
#endif