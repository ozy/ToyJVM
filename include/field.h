#ifndef FIELD_H_
#define FIELD_H_
#include <inttypes.h>
#include "attribute.h"
#include <stdio.h>
typedef enum access_flags{
    ACC_PUBLIC 	    = 0x0001, // Declared public; may be accessed from outside its package.
    ACC_PRIVATE 	= 0x0002, // Declared private; usable only within the defining class.
    ACC_PROTECTED 	= 0x0004, // Declared protected; may be accessed within subclasses.
    ACC_STATIC 	    = 0x0008, // Declared static.
    ACC_FINAL 	    = 0x0010, // Declared final; never directly assigned to after object construction (JLS §17.5).
    ACC_VOLATILE 	= 0x0040, // Declared volatile; cannot be cached.
    ACC_TRANSIENT 	= 0x0080, // Declared transient; not written or read by a persistent object manager.
    ACC_SYNTHETIC 	= 0x1000, // Declared synthetic; not present in the source code.
    ACC_ENUM 	    = 0x4000  // Declared as an element of an enum. 

}access_flags;

typedef struct field_info {
    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    uint16_t attributes_count;
    attribute_info* attributes;
}field_info;

field_info getField_Info(FILE* fd);

#endif