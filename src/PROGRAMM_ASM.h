#ifndef ASSEMBLER_INCLUDED
#define ASSEMBLER_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<stdint.h>
#include<assert.h>
#include<ctype.h>

#include"../MyStack/StackFunc.h"
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
const long COMMANDNAME_MAX = 32;
const size_t ARGLEN_MAX = 64;
static const size_t default_max_code_size = 128;

const char CommandNames[32][COMMANDNAME_MAX]
{"", "push", "", "add", "sub", "div", "mul", "pow",
 "sqrt", "sin", "in", "pop", "", "", "", "", "out", "",
 "", "", "jmp", "jb", "ja","je", "jne", "jbe", "jae", "call", "ret",
 "dump", "", "hlt"};



typedef struct asm_sheet
{
    char* source;
    size_t size;
} asmsht;


typedef struct ASM_t
{
    Code_t* code;
    uint32_t code_size;
    Stack_t stk;
    LabelTable LTable;

    asmsht sheet;
} ASM_t;

FILE* Command_file_open(int argc, const char** argv);
int Convert_Code_To_Array(ASM_t* Asm);
int Write_in_file(FILE* Output_code, ASM_t* Asm);
int32_t Register_convert(char* buffer);

int Bufferize_file(FILE* CODE_ASM, ASM_t* Asm);
long GetFileSize(FILE* file);
size_t CodeResize(ASM_t* Asm, size_t oldcapacity);
int GetCommand(ASM_t* Asm, char* buffer, size_t* been_read);
long CommandFind(char* buffer);

int ArgPush(ASM_t* Asm, char* buffer, size_t* size_of_code);
int ArgJump(ASM_t* Asm, char* buffer, size_t* size_of_code);
int ArgCall(ASM_t* Asm, char* buffer, size_t* size_of_code);
int ArgLabel(ASM_t* Asm, char* buffer, size_t size_of_code, char* lmarker);
int ArgPop(ASM_t* Asm, char* buffer, size_t* size_of_code);

int AsmCtor(ASM_t* Asm);
int AsmDtor(ASM_t* Asm);

#endif
