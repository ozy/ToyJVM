#ifndef CLASSFILE_H_
#define CLASSFILE_H_
#include <inttypes.h>
#include "constantPool.h"
#include "field.h"
#include "method.h"


typedef struct ClassFile{
    uint32_t       magic;
    uint16_t       minor_version;
    uint16_t       major_version;
    uint16_t       constant_pool_count;
    cp_info*       constant_pool; //[constant_pool_count-1]
    uint16_t       access_flags;
    uint16_t       this_class;
    uint16_t       super_class;
    uint16_t       interfaces_count;
    uint16_t*      interfaces;
    uint16_t       fields_count;
    field_info*    fields;
    uint16_t       methods_count;
    method_info*   methods;
    uint16_t       attributes_count;
    attribute_info* attributes;
}ClassFile;

ClassFile fromClassFile(unsigned char* filename);

#endif