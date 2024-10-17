#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCKUDED

#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#include"StackFunc.h"

enum registers
{
    AX  = 1,
    BX  = 2,
    CX  = 3,
    DX  = 4,
    MLR = 5,
};

enum Machine_Commands
{
    PUSH  = 1,
    OUT   = 2,
    ADD   = 3,
    SUB   = 4,
    MUL   = 5,
    DIV   = 6,
    DUMP  = 7,

    JMP   = 8,
    JA    = 9,
    JAE   = 10,
    JB    = 11,
    JBE   = 12,
    JE    = 13,
    JNE   = 14,

    PUSHR = 15,
    POP   = 16,

    HLT   = 0,
};

typedef int Code_t;

typedef struct SPU_t
{
    Code_t code[512];
    size_t ip;
    size_t code_size;

    Stack_t stk;

    int registers[16] ;
} SPU_t;


int SPUDump_t(SPU_t* spu, const char* file, const char* func, int line);

#ifndef SPU_DUMP_INC
#define SpuDump(spu) SPUDump_t((spu),  __FILE__, __func__, __LINE__)
#endif

#endif
