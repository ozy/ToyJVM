#ifndef JAVACLASS_H_
#define JAVACLASS_H_
#include "classFile.h"

typedef struct JavaClass{
    ClassFile* classFile;
    field_info* fields;
}JavaClass;

void initInstanceFields(JavaClass* instance);

#endif