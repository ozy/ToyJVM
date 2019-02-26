#ifndef MACHINE_H_
#define MACHINE_H_
#include "frame.h"
#include "opcode.h"

void* executeCode(Frame frame, OPCODE** opcodes);


#endif