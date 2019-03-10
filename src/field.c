#include "field.h"
#include <stdio.h>
#include <endian.h>
#include <stdlib.h>
#include "classFile.h"
#include "constantPool.h"
#include "javaClass.h"

field_info getField_Info(FILE* fd){
    field_info field;
    fread(&field.access_flags,sizeof(field.access_flags),1,fd);
    field.access_flags = be16toh(field.access_flags);

    fread(&field.name_index,sizeof(field.name_index),1,fd);
    field.name_index = be16toh(field.name_index);

    fread(&field.descriptor_index,sizeof(field.descriptor_index),1,fd);
    field.descriptor_index = be16toh(field.descriptor_index);

    fread(&field.attributes_count,sizeof(field.attributes_count),1,fd);
    field.attributes_count = be16toh(field.attributes_count);

    field.attributes = malloc (sizeof(attribute_info) * field.attributes_count);
    for (int attributeCount=0; attributeCount<field.attributes_count; attributeCount++){
        field.attributes[attributeCount] = getAttribute_Info(fd); // leak
    }
    field.value = 0;
    return field;

}

void destroyField_Info(field_info* field){
    for (; field->attributes_count > 0; field->attributes_count--)
        destroyAttribute_Info(&field->attributes[field->attributes_count-1]);
    free(field->attributes);
}

void putField(JavaClass* instance, CONSTANT_Utf8_info fieldName, CONSTANT_Utf8_info fieldDesc, uint64_t val){
    field_info* field = getField(instance, fieldName, fieldDesc);
    field->value = val;
}


field_info* getField(JavaClass* jc, CONSTANT_Utf8_info fieldName, CONSTANT_Utf8_info fieldDesc){
    for (uint16_t fieldId=0; fieldId<jc->classFile->fields_count; fieldId++){
        field_info* field = &jc->fields[fieldId];
        CONSTANT_Utf8_info _fieldName = jc->classFile->constant_pool[field->name_index-1].info.utf8_info;
        CONSTANT_Utf8_info _fieldDesc = jc->classFile->constant_pool[field->descriptor_index-1].info.utf8_info;
        if (isUtf8Equal(fieldName,_fieldName) && isUtf8Equal(fieldDesc,_fieldDesc)){
            return field;
        }
    }
    return NULL;
}

field_info* getStaticField(ClassFile* cf, CONSTANT_Utf8_info fieldName, CONSTANT_Utf8_info fieldDesc){
    for (uint16_t fieldId=0; fieldId<cf->fields_count; fieldId++){
        field_info* field = &cf->fields[fieldId];
        CONSTANT_Utf8_info _fieldName = cf->constant_pool[field->name_index-1].info.utf8_info;
        CONSTANT_Utf8_info _fieldDesc = cf->constant_pool[field->descriptor_index-1].info.utf8_info;
        if (isUtf8Equal(fieldName,_fieldName) && isUtf8Equal(fieldDesc,_fieldDesc)){
            return field;
        }
    }
    return NULL;
}