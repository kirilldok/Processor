#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCKUDED

#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#include<assert.h>

#include"StackFunc.h"

enum ErrCodes
{
    NO_ERROR,
    FOPEN_ERROR,
    ASSEMBLER_ERROR,
    READING_ERROR,
    BUFFERISATION_ERROR,
    COMPILER_ERROR,
    STACK_INIT_ERROR,
    PUSH_ERROR,
    POP_ERROR,
    HEADER_READ_ERROR,
};

typedef int Code_t;



const uint32_t RAM_MASK = 4;
const uint32_t REG_MASK = 2;
const uint32_t C_MASK = 1;


typedef struct SPU_t
{
    char* code;
    size_t ip;
    int code_size;

    Stack_t stk;

    Code_t registers[16];
    int ram[512];
} SPU_t;

int SpuCtor(SPU_t* spu);
int SpuDtor(SPU_t* spu);
int SPUDump(SPU_t* spu, const char* file, const char* func, int line);

#ifndef SPU_DUMP_INC
#define SpuDump(spu) SPUDump((spu),  __FILE__, __func__, __LINE__)
#endif

#endif
