#include "stringBuilder.h"
#include <inttypes.h>
#include "constantPool.h"
#include "frame.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "debug.h"

void handleStringBuilder(uint64_t* localVariables, Frame* frame, CONSTANT_Utf8_info name_utf8, CONSTANT_Utf8_info desc_utf8){
    cp_info* strRef = (cp_info*)hGet(localVariables[0],frame->machine->heap);
    if (isUtf8EqualsToString(name_utf8,"append")){
        if (isUtf8EqualsToString(desc_utf8,"(I)Ljava/lang/StringBuilder;") || isUtf8EqualsToString(desc_utf8,"(J)Ljava/lang/StringBuilder;")){
            int stringLength = floor((*(int64_t*)&localVariables[1]==0)?0:log10(abs(*(int64_t*)&localVariables[1]))) + 1;
            strRef->info.utf8_info.bytes = (char*)hExtend((uint64_t)(strRef->info.utf8_info.bytes), stringLength, frame->machine->heap);
            char* charRef = hGet((uint64_t)strRef->info.utf8_info.bytes, frame->machine->heap);
            sprintf(charRef,"%s%d",charRef,*(int64_t*)&localVariables[1]);
            strRef->info.utf8_info.length += stringLength;
            DEBUG_PRINT(("append test: %s\n", charRef));

        }else if(isUtf8EqualsToString(desc_utf8,"(Ljava/lang/String;)Ljava/lang/StringBuilder;")){
            cp_info* appendRef = (cp_info*)localVariables[1];
            CONSTANT_Utf8_info appendStrRef;
            if (appendRef->tag == CONSTANT_String){
                appendStrRef = frame->classRef->constant_pool[appendRef->info.string_info.string_index-1].info.utf8_info;
            }else{
                appendStrRef = appendRef->info.utf8_info;
            }
            strRef->info.utf8_info.bytes = (char*)hExtend((uint64_t)(strRef->info.utf8_info.bytes), appendStrRef.length, frame->machine->heap);
            char* charRef = hGet((uint64_t)strRef->info.utf8_info.bytes, frame->machine->heap);
            strncat(charRef,appendStrRef.bytes,appendStrRef.length);
            strRef->info.utf8_info.length += appendStrRef.length;
            DEBUG_PRINT(("append test: %s, strLen: %d, ldc: %.*s\n", charRef,frame->machine->heap->heap[(uint64_t)strRef->info.utf8_info.bytes].size, appendStrRef.length, appendStrRef.bytes));
        }
        pushStack(&localVariables[0], frame->operandStack);
    }else if(isUtf8EqualsToString(name_utf8,"toString")){
        cp_info* asString = malloc(sizeof(cp_info));
        asString->tag = CONSTANT_Utf8;
        asString->info.utf8_info.length = strRef->info.utf8_info.length;
        asString->info.utf8_info.bytes = hGet((uint64_t)strRef->info.utf8_info.bytes, frame->machine->heap);
        pushStack(&asString, frame->operandStack);
    }
}
