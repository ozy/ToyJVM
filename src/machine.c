#include "machine.h"
#include "frame.h"
#include "opcode.h"
#include "stack.h"

void* executeCode(Stack* JVMSTACK, OPCODE** opcodes){ // anything can be returned from an execution

    Frame* frame;
    void* retCode;
    while (1){
        frame = (Frame*)peekStack(JVMSTACK);
        //printf("---------------\n");
        //printf("peeked stack is %p\n",frame);

        //printf("---Code Dump---\n");
        //for (int qq=0; qq<frame->code->code_length; qq++)
        //    printf("%d: 0x%1x\n", qq,frame->code->code[qq]);
        //printf("-----DUMP------\n");

        for (; frame->pc < frame->code->code_length; frame->pc++){
            retCode = NULL;
            //printf("pc: %d -> 0x%1x, opStackTop: %d\n",frame->pc , frame->code->code[frame->pc],frame->operandStack->top);
            retCode = opcodes[frame->code->code[frame->pc]](frame);
            if (retCode != NULL){
                // the opcode returned something to push
                // to the invoker operand stack
                //printf("retcode is not null\n");
                popStack(JVMSTACK); //destroy completely
                break;
            }
            if (frame != (Frame*)peekStack(JVMSTACK)){
                break; // frame changed, break!
            }
        }
        if (peekStack(JVMSTACK) == NULL){
            printf("no frame left to execute\n");
            return NULL;
        }else if (retCode != NULL){
            Frame* frameInvoker = (Frame*)peekStack(JVMSTACK);
            pushStack(retCode,frameInvoker->operandStack);
        }else if (frame->pc >= frame->code->code_length-1){
            popStack(JVMSTACK); //destroy completely
        }
    }
}