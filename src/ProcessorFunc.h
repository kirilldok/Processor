#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCKUDED

#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#include<assert.h>

#include"../MyStack/StackFunc.h"
#include"Commands.h"

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


const uint32_t RAM_MASK = 4;
const uint32_t REG_MASK = 2;
const uint32_t C_MASK = 1;
static const size_t RAM_SIZE = 8;

static const size_t default_max_code_size = 256;


typedef struct SPU_t
{
    Code_t* code;
    size_t ip;
    size_t code_size;

    Stack_t stk;
    Stack_t return_codes;

    Code_t registers[16];
    Code_t ram[RAM_SIZE];
} SPU_t;

int SpuCtor(SPU_t* spu);
size_t CodeResize(SPU_t* spu, size_t oldcapacity);
int SpuDtor(SPU_t* spu);
int SPUDump(SPU_t* spu, const char* file, const char* func, int line);

#ifndef SPU_DUMP_INC
#define SpuDump(spu) SPUDump((spu),  __FILE__, __func__, __LINE__)
#endif

#endif
