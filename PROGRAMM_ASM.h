#ifndef ASSEMLER_INCLUDED
#define ASSEMBLER_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<stdint.h>

#include<LableTable>

const uint32_t sign = 0x38EEE96B;
const uint32_t ver = 2;
const size_t COMMANDNAME_MAX = 32;
const size_t ARGLEN_MAX = 64;

const char CommandNames[32][COMMANDNAME_MAX]
{"", "push", "pushr", "add", "sub", "div", "mul", "pow",
 "sqrt", "sin", "", "pop", "", "", "", "", "out", "dump",
 "", "", "jmp", "jb", "ja","je", "jne", "jbe", "jae", "", "",
 "", "", "hlt"};

typedef int Code_t;

typedef struct ASM_t
{
    Code_t* code;
    unsigned code_size;

    Stack_t stk;
    LabelTable LTable;

} ASM_t;

int Convert_Code_To_Array(FILE* Input_code, SPU_t* code);
int Write_in_file(FILE* Output_code, SPU_t* spu);
int Register_convert(char str[]);
size_t GetFileSize(FILE* file);
int AsmPush(Spu_t* spu, char* buffer, int* size_of_code);
int AsmJump(SPU_t* spu, char* buffer, int* size_of_code);

#endif
