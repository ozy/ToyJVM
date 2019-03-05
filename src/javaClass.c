#include "javaClass.h"
#include <stdlib.h>

void initInstanceFields(JavaClass* instance){
    instance->fields = malloc(instance->classFile->fields_count * sizeof(field_info));
    for (int fieldId=0; fieldId < instance->classFile->fields_count; fieldId++){
        instance->fields[fieldId] = instance->classFile->fields[fieldId];
    }
    // todo: init with memcpy
}