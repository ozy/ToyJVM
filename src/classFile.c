#include "classFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <endian.h> // check for cross platform compatibility
#include "field.h"
#include "method.h"
#include "constantPool.h"
#include "string.h"

ClassFile classFromFile(unsigned char* filename){
    FILE *fd;
    fd = fopen(filename, "r");

    if (fd == NULL){
       perror("Error while opening the file.\n");
       exit(EXIT_FAILURE);
    }

    /*
    uint32_t       magic;
    uint16_t       minor_version;
    uint16_t       major_version;
    uint16_t       constant_pool_count;
    cp_info        constant_pool[constant_pool_count-1];
    */
    ClassFile classFile;
    fread(&classFile.magic,sizeof(classFile.magic),1,fd);
    classFile.magic = be32toh(classFile.magic); // 0xCAFEBABE

    fread(&classFile.minor_version,sizeof(classFile.minor_version),1,fd);
    classFile.minor_version = be16toh(classFile.minor_version);

    fread(&classFile.major_version,sizeof(classFile.major_version),1,fd);
    classFile.major_version = be16toh(classFile.major_version);

    fread(&classFile.constant_pool_count,sizeof(classFile.constant_pool_count),1,fd);
    classFile.constant_pool_count = be16toh(classFile.constant_pool_count);

    classFile.constant_pool = malloc (sizeof(cp_info) * (classFile.constant_pool_count-1)); // leak
    // The value of the constant_pool_count item is equal to the number of entries in the constant_pool table plus one.
    //A constant_pool index is considered valid if it is greater than zero and less than constant_pool_count, 
    //with the exception for constants of type long and double noted in §4.4.5.

    for (int poolCount=0; poolCount < classFile.constant_pool_count-1; poolCount++){ // -1 same reason above
        classFile.constant_pool[poolCount] = cp_infoFromFile(fd); // leak 
    }

    /*
    u2             access_flags;
    u2             this_class;
    u2             super_class;
    u2             interfaces_count;
    u2             interfaces[interfaces_count];
    u2             fields_count;
    field_info     fields[fields_count];
    */

    fread(&classFile.access_flags,sizeof(classFile.access_flags),1,fd);
    classFile.access_flags = be16toh(classFile.access_flags);

    fread(&classFile.this_class,sizeof(classFile.this_class),1,fd);
    classFile.this_class = be16toh(classFile.this_class);

    fread(&classFile.super_class,sizeof(classFile.super_class),1,fd);
    classFile.super_class = be16toh(classFile.super_class);

    fread(&classFile.interfaces_count,sizeof(classFile.interfaces_count),1,fd);
    classFile.interfaces_count = be16toh(classFile.interfaces_count);

    classFile.interfaces = malloc(classFile.interfaces_count * sizeof (uint16_t)); // leak
    for (int interfaceCount=0; interfaceCount < classFile.interfaces_count; interfaceCount++){
        uint16_t interface;
        fread(&interface,sizeof(interface),1,fd);
        classFile.interfaces[interfaceCount] = be16toh(interface);
    }
    
    fread(&classFile.fields_count,sizeof(classFile.fields_count),1,fd);
    classFile.fields_count = be16toh(classFile.fields_count);

    classFile.fields = malloc(sizeof(field_info) * classFile.fields_count); // leak
    for (int fieldCount=0; fieldCount < classFile.fields_count; fieldCount++){
        classFile.fields[fieldCount] = getField_Info(fd); // leak
    }

    /*
    u2             methods_count;
    method_info    methods[methods_count];
    u2             attributes_count;
    attribute_info attributes[attributes_count];
    */

    fread(&classFile.methods_count,sizeof(classFile.methods_count),1,fd);
    classFile.methods_count = be16toh(classFile.methods_count);

    classFile.methods = malloc (sizeof(method_info) * classFile.methods_count); // leak
    for (int methodCount = 0; methodCount<classFile.methods_count; methodCount++){
        classFile.methods[methodCount] = getMethod_Info(fd); // leak
    }

    fread(&classFile.attributes_count,sizeof(classFile.attributes_count),1,fd);
    classFile.attributes_count = be16toh(classFile.attributes_count);

    classFile.attributes = malloc (sizeof(attribute_info) * classFile.attributes_count); // leak
    for (int attributeCount = 0; attributeCount<classFile.attributes_count; attributeCount++){
        classFile.attributes[attributeCount] = getAttribute_Info(fd); // leak
    }

    return classFile;
}
char isUtf8Equal(CONSTANT_Utf8_info c1, CONSTANT_Utf8_info c2){
    if (c1.length != c2.length)
        return 0; // false
    return !strncmp(c1.bytes,c2.bytes,c1.length);
}

int getNumArgs(ClassFile* cf, CONSTANT_Ref_info methodOrInterfaceRef){
    CONSTANT_NameAndType_info* nameAndType = &cf->constant_pool[methodOrInterfaceRef.name_and_type_index-1].info.nameAndType_info;
    CONSTANT_Utf8_info descriptor_utf8 = cf->constant_pool[nameAndType->descriptor_index-1].info.utf8_info;
    int numArgs = 0;
    for (int index=0; index < descriptor_utf8.length; index++){
        switch (descriptor_utf8.bytes[index]){
            case 'L':
                for (; descriptor_utf8.bytes[index] != ';'; index++){
                }
                numArgs++;
                break;
            case '(':
                break;
            case ')':
                return numArgs;
            default:
                numArgs++;
                break;
        }
    }
    return numArgs;
}

ClassFile* getClassFromName(CONSTANT_Utf8_info utf8, Machine machine){
    // todo BTree
    for (int classId=0; classId<machine.numClasses; classId++){
        ClassFile* class = &machine.classFiles[classId];
        if (isUtf8Equal(class->constant_pool[class->this_class-1].info.utf8_info, utf8)){
            return class;
        }
    }
    return NULL;
}

method_info* canClassHandleMethod(ClassFile* cf, CONSTANT_Ref_info methodOrInterfaceRef){
    CONSTANT_NameAndType_info* nameAndType = &cf->constant_pool[methodOrInterfaceRef.name_and_type_index-1].info.nameAndType_info;
    CONSTANT_Utf8_info name_utf8 = cf->constant_pool[nameAndType->name_index-1].info.utf8_info;
    CONSTANT_Utf8_info descriptor_utf8 = cf->constant_pool[nameAndType->descriptor_index-1].info.utf8_info; // clear later
    printf ("Invoked Method Name: %.*s, %.*s\n",name_utf8.length,name_utf8.bytes, descriptor_utf8.length,descriptor_utf8.bytes);
    for (int methodId=0; methodId < cf->methods_count; methodId++){
        CONSTANT_Utf8_info classMethodName_utf8 = cf->constant_pool[cf->methods[methodId].name_index-1].info.utf8_info; // todo possibly optimize this func
        if (isUtf8Equal(name_utf8, classMethodName_utf8)){
            //printf ("Found Method Name: %.*s\n",classMethodName_utf8.length,classMethodName_utf8.bytes);
            return &cf->methods[methodId];
        }
    }
    return NULL; // method is not in this class
}

char checkFormat(ClassFile cf){
    //1
    if (cf.magic != 0xCAFEBABE)
        return 0;
    
    //2
    for (int attributeCount=0; attributeCount<cf.attributes_count; attributeCount++){
        uint16_t utf8Index = cf.attributes[attributeCount].attribute_name_index;
        if (cf.constant_pool[utf8Index-1].tag != CONSTANT_Utf8){ // constant pool starts from 1 (lol, bcz oracle said)
            //The constant_pool entry at attribute_name_index must be a CONSTANT_Utf8_info structure
            return 0;
        }
    }
    //4
    for (int constantPoolCount=0; constantPoolCount < cf.constant_pool_count-1; constantPoolCount++){
        switch (cf.constant_pool[constantPoolCount].tag){
            case CONSTANT_Class:; // empty statement ;
                uint16_t utf8Index = cf.constant_pool[constantPoolCount].info.class_info.name_index;
                if (cf.constant_pool[utf8Index-1].tag != CONSTANT_Utf8)
                    return 0;
                break;
            case CONSTANT_Fieldref:;
                // The class_index item of a CONSTANT_Fieldref_info structure may be either a class type or an interface type. 
                CONSTANT_Ref_info fieldref = cf.constant_pool[constantPoolCount].info.ref_info;
                if (!(cf.constant_pool[fieldref.class_index-1].tag == CONSTANT_Class || cf.constant_pool[fieldref.class_index-1].tag == CONSTANT_InterfaceMethodref))
                    return 0;
                break;
            case CONSTANT_InterfaceMethodref:;
                CONSTANT_Ref_info interfaceMethodref = cf.constant_pool[constantPoolCount].info.ref_info;
                if (cf.constant_pool[interfaceMethodref.class_index-1].tag != CONSTANT_InterfaceMethodref)
                    return 0;
                break;
            case CONSTANT_Methodref:;
                // The class_index item of a CONSTANT_Fieldref_info structure may be either a class type or an interface type. 
                CONSTANT_Ref_info methodref = cf.constant_pool[constantPoolCount].info.ref_info;
                if (cf.constant_pool[methodref.class_index-1].tag != CONSTANT_Class)
                    return 0;
                break;
            default:
                break;
        }
    }
    return 1;
}