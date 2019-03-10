#include "javaClass.h"
#include <stdlib.h>
#include "heap.h"
#include <string.h>

void initInstanceFields(JavaClass* instance){
    instance->fields = malloc(instance->classFile->fields_count * sizeof(field_info));
    memcpy(instance->fields, instance->classFile->fields, instance->classFile->fields_count * sizeof(field_info));
}