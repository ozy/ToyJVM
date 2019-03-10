#include "printStream.h"
#include <inttypes.h>
#include "constantPool.h"
#include "frame.h"
#include <stdlib.h>


void handlePrintStream(uint64_t* localVariables, Frame* frame, CONSTANT_Utf8_info name_utf8, CONSTANT_Utf8_info desc_utf8){
    if (isUtf8EqualsToString(desc_utf8,"(I)V")){
        printf("%d\n",localVariables[1]);
        return;
    }
    cp_info* printArg = (cp_info*)localVariables[1]; // 0 is ref, 1 is arg1
    switch (printArg->tag)
    {
        case CONSTANT_Utf8: // created from native toStrings, needs to be freed after use.
            printf("%.*s\n",printArg->info.utf8_info.length,printArg->info.utf8_info.bytes);
            free(printArg);
            break;
        case CONSTANT_String:;
            CONSTANT_Utf8_info str = frame->classRef->constant_pool[printArg->info.string_info.string_index-1].info.utf8_info;
            printf("%.*s\n",str.length,str.bytes);
            break;
        default:
            printf("OBJECT, TYPE: %d, @ %p\n", printArg->tag, printArg);
            break;
    }
}