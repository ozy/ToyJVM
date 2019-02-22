#ifndef METHOD_H_
#define METHOD_H_
#include "attribute.h"
#include "field.h"

typedef struct field_info method_info; // same as field info

#define getMethod_Info(fd) getField_Info(fd) // same too, no need to reimplement

#endif
