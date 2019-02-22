#include "attribute.h"
#include <stdlib.h>
#include <endian.h>

attribute_info getAttribute_Info(FILE* fd){
    attribute_info attribute;
    fread(&attribute.attribute_name_index,sizeof(attribute.attribute_name_index),1,fd);
    attribute.attribute_name_index = be16toh(attribute.attribute_name_index);

    fread(&attribute.attribute_length,sizeof(attribute.attribute_length),1,fd);
    attribute.attribute_length = be32toh(attribute.attribute_length);

    attribute.info = malloc(attribute.attribute_length); // leak
    fread(attribute.info,attribute.attribute_length,1,fd);
    return attribute;
}

