#ifndef ASSEMLER_INCLUDED
#define ASSEMBLER_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<stdint.h>
#include<assert.h>
#include<ctype.h>

#include"StackFunc.h"
#include"LableTable.h"
#include"Commands.h"


enum ErrCodes
{
    FOPEN_ERROR = 1,
    ASSEMBLER_ERROR,
    READING_ERROR,
    LTABLE_CREATION_ERROR,
    CALLOC_ERROR,
    BINARY_WRITING_ERROR,
    UNDEFINED_LABEL,
    STACK_CREATION_ERROR,
    SYNTAX_ERROR,
};

const uint32_t sign = 0x38EEE96B;
const uint32_t ver = 2;
const size_t COMMANDNAME_MAX = 32;
const size_t ARGLEN_MAX = 64;
static const size_t default_max_code_size = 128;

const char CommandNames[32][COMMANDNAME_MAX]
{"", "push", "pushr", "add", "sub", "div", "mul", "pow",
 "sqrt", "sin", "", "pop", "", "", "", "", "out", "dump",
 "", "", "jmp", "jb", "ja","je", "jne", "jbe", "jae", "", "",
 "", "", "hlt"};


typedef struct ASM_t
{
    char* code;
    int code_size;

    Stack_t stk;
    LabelTable LTable;

} ASM_t;

int Convert_Code_To_Array(FILE* Input_code, ASM_t* code);
int Write_in_file(FILE* Output_code, ASM_t* Asm);
int Register_convert(char str[]);

size_t GetFileSize(FILE* file);
char CommandFind(char* buffer);

int ArgPush(ASM_t* Asm, char* buffer, int* size_of_code);
int ArgJump(ASM_t* Asm, char* buffer, int* size_of_code);
int ArgLabel(ASM_t* Asm, char* lmarker, char* buffer, int size_of_code);
int ArgPop(ASM_t* Asm, char* buffer, int* size_of_code);
int AsmCtor(ASM_t* Asm);
int AsmDtor(ASM_t* Asm);

#endif
