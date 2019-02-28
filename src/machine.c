#include "machine.h"
#include "frame.h"
#include "opcode.h"
#include "stack.h"

void* executeCode(Stack* JVMSTACK, OPCODE** opcodes){ // anything can be returned from an execution

    Frame* frame;
    void* retCode;
    while (1){
        frame = (Frame*)peekStack(JVMSTACK);
        printf("peeked stack is %p\n",frame);

        printf("---Code Dump---\n");
        for (int qq=0; qq<frame->code->code_length; qq++)
            printf("%d: 0x%1x\n", qq,frame->code->code[qq]);
        printf("-----DUMP------\n");

        for (; frame->pc < frame->code->code_length; frame->pc++){
            printf ("current frame pc: %d\n",frame->pc);
            retCode = NULL;
            //printf("0x%1x, opStackTop: %d\n", frame->code->code[frame->pc],frame->operandStack->top);
            retCode = opcodes[frame->code->code[frame->pc]](frame);
            printf("---------\n");
            if (retCode != NULL){
                // the opcode returned something to push
                // to the invoker operand stack
                popStack(JVMSTACK); //destroy completely
                break;
            }
            if (frame != (Frame*)peekStack(JVMSTACK)){
                printf("frame changed\n");
                break; // frame changed, break!
            }
        }
        if (peekStack(JVMSTACK) == NULL){
            return NULL;
        }else if (retCode != NULL){
            Frame* frameInvoker = (Frame*)peekStack(JVMSTACK);
            pushStack(retCode,frameInvoker->operandStack);
        }
    }
}