#ifndef STRINGBUILDER_H_
#define STRINGBUILDER_H_
#include <inttypes.h>
#include "frame.h"
#include "classFile.h"


void handleStringBuilder(uint64_t* localVariables, Frame* frame, CONSTANT_Utf8_info name_utf8, CONSTANT_Utf8_info desc_utf8);
 
#endif