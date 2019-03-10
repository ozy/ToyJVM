#include "frame.h"
#include <stdlib.h>
#include "classFile.h"
#include "attribute.h"

Frame createNewFrame (Code_attribute code, ClassFile* cf, Machine* machine){
    Frame newFrame;
    newFrame.code = malloc (sizeof(Code_attribute));
    *newFrame.code = code;

    newFrame.localVariables = malloc (sizeof(LocalVariable) * newFrame.code->max_locals);

    newFrame.operandStack = malloc (sizeof(Stack));
    *newFrame.operandStack = initStack(newFrame.code->max_stack, TYPE_OPERANDSTACK);

    newFrame.pc = 0;
    newFrame.classRef = cf;
    newFrame.machine = machine;
    return newFrame;
}

void destroyFrame (Frame* frame){
    free(frame->localVariables);
    free(frame->code);
    destroyStack(frame->operandStack);
    // dont free the frame struct itself. It contains an address into JVMStack.
    // JVMStack is responsible for freeing or overwriting it.
}