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

char checkFormat(ClassFile cf);
/*
Returns 1 if the class file passes the test. Anything else if not.

1-The first four bytes must contain the right magic number.+
2-All recognized attributes must be of the proper length.?
3-The class file must not be truncated or have extra bytes at the end.-
4-The constant pool must satisfy the constraints documented throughout §4.4.?
For example, each CONSTANT_Class_info structure in the constant pool 
must contain in its name_index item a valid constant pool index for a CONSTANT_Utf8_info structure.
6-All field references and method references in the constant pool must 
have valid names, valid classes, and valid descriptors (§4.3).
7-Format checking does not ensure that the given field or method actually 
exists in the given class, nor that the descriptors given refer to real classes. 
Format checking ensures only that these items are well formed. More detailed 
checking is performed when the bytecodes themselves are verified, and during resolution.
*/
#endif