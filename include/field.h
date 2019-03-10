#ifndef FIELD_H_
#define FIELD_H_
#include <inttypes.h>
#include "attribute.h"
#include <stdio.h>
#include "classFile.h"
#include "constantPool.h"

typedef struct JavaClass JavaClass;
typedef enum field_access_flags{
    F_ACC_PUBLIC 	    = 0x0001, // Declared public; may be accessed from outside its package.
    F_ACC_PRIVATE 	    = 0x0002, // Declared private; usable only within the defining class.
    F_ACC_PROTECTED 	= 0x0004, // Declared protected; may be accessed within subclasses.
    F_ACC_STATIC 	    = 0x0008, // Declared static.
    F_ACC_FINAL 	    = 0x0010, // Declared final; never directly assigned to after object construction (JLS §17.5).
    F_ACC_VOLATILE 	    = 0x0040, // Declared volatile; cannot be cached.
    F_ACC_TRANSIENT 	= 0x0080, // Declared transient; not written or read by a persistent object manager.
    F_ACC_SYNTHETIC 	= 0x1000, // Declared synthetic; not present in the source code.
    F_ACC_ENUM 	        = 0x4000  // Declared as an element of an enum. 
}access_flags;

typedef struct field_info {
    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    uint16_t attributes_count;
    attribute_info* attributes;
    uint64_t value;
}field_info;

field_info getField_Info(FILE* fd);

field_info* getStaticField(ClassFile* cf, CONSTANT_Utf8_info fieldName, CONSTANT_Utf8_info fieldDesc);
field_info* getField(JavaClass* jc, CONSTANT_Utf8_info fieldName, CONSTANT_Utf8_info fieldDesc);
void putField(JavaClass* instance, CONSTANT_Utf8_info fieldName, CONSTANT_Utf8_info fieldDesc, uint64_t val);

void destroyField_Info(field_info* field);
#endif