#ifndef CONSTANTPOOL_H_
#define CONSTANTPOOL_H_
#include <inttypes.h>
#include <stdio.h>

typedef enum cp_tags{
    CONSTANT_Class=7,
    CONSTANT_Fieldref=9,
    CONSTANT_Methodref=10,
    CONSTANT_InterfaceMethodref=11,
    CONSTANT_String=8,
    CONSTANT_Integer=3,
    CONSTANT_Float=4,
    CONSTANT_Long=5,
    CONSTANT_Double=6,
    CONSTANT_NameAndType=12,
    CONSTANT_Utf8=1,
    CONSTANT_MethodHandle=15,
    CONSTANT_MethodType=16,
    CONSTANT_InvokeDynamic=18
}cp_tags;

typedef struct cp_info {
    uint8_t tag;
    uint8_t* info;
}cp_info;


//////////////////////////////
// CONSTANT INFO STRUCTURES //
//////////////////////////////

// the data written directly into the structures.
// padding across the platforms should be
// checked.
typedef struct CONSTANT_Class_info {
    uint16_t name_index;
}CONSTANT_Class_info;

typedef struct CONSTANT_Methodref_info{
    // also applies for The CONSTANT_Fieldref_info, CONSTANT_Methodref_info, and CONSTANT_InterfaceMethodref_info Structures
    uint16_t class_index;
    uint16_t name_and_type_index;
}CONSTANT_Methodref_info;
typedef struct CONSTANT_Methodref_info CONSTANT_Fieldref_info;
typedef struct CONSTANT_Methodref_info CONSTANT_InterfaceMethodref_info;


typedef struct CONSTANT_String_info {
    uint16_t string_index;
}CONSTANT_String_info;

typedef struct CONSTANT_Integer_info {
    uint32_t bytes;
}CONSTANT_Integer_info;
typedef struct CONSTANT_Integer_info CONSTANT_Float_info;

typedef struct CONSTANT_Long_info {
    uint64_t bytes;
}CONSTANT_Long_info;
typedef struct CONSTANT_Long_info CONSTANT_Double_info;
// to convert long: (for big endian)
// ((long) high_bytes << 32) + low_bytes

typedef struct CONSTANT_NameAndType_info {
    uint16_t name_index;
    uint16_t descriptor_index;
}CONSTANT_NameAndType_info;

typedef struct CONSTANT_Utf8_info {  // not aligned
    uint16_t length;
    // 6 bytes padding
    uint8_t* bytes;
}CONSTANT_Utf8_info;

typedef struct CONSTANT_MethodHandle_info { // not aligned
    uint8_t reference_kind; // ref to method handle.
    // 1 byte compiler padding
    uint16_t reference_index; 
}CONSTANT_MethodHandle_info;

typedef struct CONSTANT_MethodType_info {
    uint16_t descriptor_index;
    //The constant_pool entry at that index must be a CONSTANT_Utf8_info structure (§4.4.7) 
    //representing a method descriptor (§4.3.3). 
}CONSTANT_MethodType_info;

typedef struct CONSTANT_InvokeDynamic_info {
    uint16_t bootstrap_method_attr_index;
    uint16_t name_and_type_index;
}CONSTANT_InvokeDynamic_info;

union CONSTANT_INFO {
    CONSTANT_Class_info class_info;
    CONSTANT_Methodref_info methodref_info;
    CONSTANT_Fieldref_info  fieldref_info;
    CONSTANT_InterfaceMethodref_info    interfaceMethodref_info;
    CONSTANT_String_info    string_info;
    CONSTANT_Integer_info   integer_info;
    CONSTANT_Float_info float_info;
    CONSTANT_Long_info  long_info;
    CONSTANT_Double_info    double_info;
    CONSTANT_NameAndType_info   nameAndType_info;
    CONSTANT_Utf8_info  utf8_info;
    CONSTANT_MethodHandle_info  methodHandle_info;
    CONSTANT_InvokeDynamic_info invodeDynamic_info;
}; // unified for runtime constant pool

typedef struct constant{
    cp_tags tag;
    union CONSTANT_INFO info;
}constant;

cp_info cp_infoFromFile(FILE* fd);

#endif