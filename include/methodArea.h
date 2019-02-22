#ifndef METHODAREA_H_
#define METHODAREA_H_
#include "heap.h"
#include "exceptions.h"
#include "inttypes.h"

typedef struct JField{
    uint64_t value;
}JField;
typedef struct JMethod{
    
}JMethod;

typedef struct JClass{
    JField* fields;
    JMethod* methods;
}JClass;

//The class area associates names of classes to instances of JClass
JClass getJClass(unsigned char* name, Heap methodArea);
Exception putJClass(unsigned char* name, JClass jClass, Heap methodArea);

JField getJField(unsigned char* name, JClass jClass);
JMethod getJMethod(unsigned char* name, JClass jClass);

Exception putJField(unsigned char* name, JClass jClass);
Exception putJMethod(unsigned char* name, JClass jClass);
#endif