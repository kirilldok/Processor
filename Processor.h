#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCKUDED

#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#include"StackFunc.h"

enum Machine_Commands
{
    PUSH = 1,
    OUT  = 10,
    ADD  = 11,
    SUB  = 100,
    MUL  = 101,
    DIV  = 110,
    DUMP = 111,
    JMP  = 1000,
    JA   = 1001,
    JAE  = 1010,
    JB   = 1011,
    JBE  = 1100,
    JE   = 1101,
    JNE  = 1110,

    HLT  = 0,
};

typedef int Code_t;

typedef struct SPU_t
{
    Code_t code[512];
    size_t ip;
    size_t code_size;

    Stack_t stk;

    int* registers;

} SPU_t;

#endif
