#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_
#include <inttypes.h>
#include <stdio.h>

typedef struct attribute_info {
    uint16_t attribute_name_index;
    uint32_t attribute_length;
    uint8_t* info;
}attribute_info;

attribute_info getAttribute_Info(FILE* fd);


#endif