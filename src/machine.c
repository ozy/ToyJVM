#include "machine.h"
#include "frame.h"
#include "opcode.h"
#include "stack.h"
#include "debug.h"
#include "classFile.h"

void* executeCode(Machine* JVM, OPCODE** opcodes){ // anything can be returned from an execution

    Frame* frame;
    void* retCode;
    while (1){
        frame = (Frame*)peekStack(JVM->JVMSTACK);
        DEBUG_PRINT(("---------------\n"));
        DEBUG_PRINT(("peeked stack is %p\n",frame));

        #ifdef DEBUG
        printf("---Code Dump---\n");
        for (int qq=0; qq<frame->code->code_length; qq++)
            printf("%d: 0x%1x\n", qq,frame->code->code[qq]);
        printf("-----DUMP------\n");
        #endif

        for (; frame->pc < frame->code->code_length; frame->pc++){
            retCode = NULL;
            DEBUG_PRINT(("pc: %d -> 0x%1x, opStackTop: %d\n",frame->pc , frame->code->code[frame->pc],frame->operandStack->top));
            retCode = opcodes[frame->code->code[frame->pc]](frame);
            if (retCode != NULL){
                // the opcode returned something to push
                // to the invoker operand stack
                //DEBUG_PRINT(("retcode is not null\n"));
                break;
            }
            if (frame != (Frame*)peekStack(JVM->JVMSTACK)){
                break; // frame changed, break!
            }
        }
        if (retCode != NULL){
            DEBUG_PRINT(("returned something, in num: %d\n", *(uint64_t*) retCode));
            destroyFrame((Frame*)popStack(JVM->JVMSTACK)); //destroy completely
            Frame* frameInvoker = (Frame*)peekStack(JVM->JVMSTACK);
            pushStack(retCode,frameInvoker->operandStack); // frame returned something to push
            free(retCode); // iret mallocs it.
            // into previous frames operand stack
        }else if (frame->pc > frame->code->code_length){
            DEBUG_PRINT(("no code left to execute in this frame pc: %d, codelen: %d\n", frame->pc, frame->code->code_length));
            destroyFrame((Frame*)popStack(JVM->JVMSTACK)); //destroy completely
            // this frame finished without returning anything
        }
        if (peekStack(JVM->JVMSTACK) == NULL){
            DEBUG_PRINT(("no frame left to execute\n"));
            free(opcodes);
            return NULL;
        }
    }
}

void destroyMachine(Machine* machine){
    for (;machine->numClasses>0; machine->numClasses--)
        destroyClass(&machine->classFiles[machine->numClasses-1]);

    free(machine->classFiles);
    destroyStack(machine->JVMSTACK);
    destroyHeap(machine->heap);
}