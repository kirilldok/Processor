#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<stdint.h>

#include "PROGRAMM_ASM.h"


int main(int argc, const char* argv[])
{
    FILE* CODE_ASM = Command_file_open(argc, argv); assert(CODE_ASM);
    ASM_t Asm = { 0 };
    AsmCtor(&Asm);
    Bufferize_file(CODE_ASM, &Asm);
    fclose(CODE_ASM);

    Convert_Code_To_Array(&Asm);
    if(Asm.LTable.lnum > 0)
    {
        //free(Asm.code); // HAVE TO CLEAN CODE
        LTDumpf(&Asm.LTable);
         if(Convert_Code_To_Array(&Asm) != 0)
        {
            fprintf(stderr, "Second assembling error\n");
            return ASSEMBLER_ERROR;
        }
    }


    FILE* Machine_code = fopen("Machine_code.bin", "w+b"); assert(Machine_code);

    if (Write_in_file(Machine_code, &Asm) != 0)
    {
        return BINARY_WRITING_ERROR;
    }

    fclose(Machine_code);
    free(Asm.sheet.source);
    AsmDtor(&Asm);

    return 0;
}


int Bufferize_file(FILE* CODE_ASM, ASM_t* Asm)
{
    Asm->sheet.size = GetFileSize(CODE_ASM);
    Asm->sheet.source = (char*)calloc(Asm->sheet.size, sizeof(Code_t));
    if(fread(Asm->sheet.source, Asm->sheet.size, sizeof(Code_t), CODE_ASM) == 0)
    {
        fprintf(stderr, "Reading error\n");
        return READING_ERROR;
    }


    for(size_t i = 0; i < Asm->sheet.size; i++)
    {
        //fprintf(stderr, "i befor: %zu\n", i);
        char* nlineptr = NULL;
        if((nlineptr = strchr(Asm->sheet.source + i, '\n')) != NULL)
        {
            *nlineptr = ' ';
            i += nlineptr - (Asm->sheet.source + i);
        }
        //fprintf(stderr, "i after: %zu\n", i);
    }

    return 0;
}


int AsmCtor(ASM_t* Asm)
{
    assert(Asm);

    Asm->code = (Code_t*)calloc(default_max_code_size, sizeof(Code_t)); assert(Asm->code);

    Asm->LTable.labAr = LTCtor(Asm->LTable.labAr, LTLENGTH_MAX); assert(Asm->LTable.labAr);
    Asm->code_size = 0;

    StackCtor(&Asm->stk, LTLENGTH_MAX); assert(&Asm->stk);

    return 0;
}


int AsmDtor(ASM_t* Asm)
{
    assert(Asm);

    StackDtor(&Asm->stk);
    LTDtor(Asm->LTable.labAr);
    free(Asm->code); Asm->code = NULL;

    return 0;
}


FILE* Command_file_open(int argc, const char** argv)
{
    FILE* code_file = NULL;

    if(argc > 1)
    {
        code_file  = fopen(argv[1], "r+b"); assert(code_file);
    }
    else
    {
        code_file = fopen("default_programm_code.txt", "r"); assert(code_file);
        fprintf(stderr, "No intput files. Default file opened.\n");
    }
    return code_file;
}


int Write_in_file(FILE* Output_code, ASM_t* Asm)
{
    assert(Output_code); assert(Asm);
    for(int i = 0; i < Asm->code_size; i++)
    {
        //fscanf(Input_code, " %d", &spu->code[spu->ip]);
        fprintf(stderr, " %lg\n", Asm->code[i]);
    }
    uint32_t hdr[size_of_header] = {sign, ver, Asm->code_size, 0};

    fwrite(hdr, sizeof(hdr[0]), size_of_header, Output_code);
    fwrite(Asm->code, sizeof(Code_t), Asm->code_size, Output_code);

    fprintf(stderr, "code has been written to file\n");
    return 0;
}



int32_t Register_convert(char* buffer)
{

    for(int i = 1; i < count_of_reg; i++)
    {
        if (strncmp(buffer, REG_NAMES[i], 2) == 0)
            return i;
    }
    //fprintf(stderr,"Error in register sintax\n");
    return -1;
}


long GetFileSize(FILE* file)
{

    fseek(file, 0L, SEEK_END);

    long size = ftell(file);

    rewind(file);
    return size;
}

int GetCommand(ASM_t* Asm, char* buffer, size_t* been_read)
{
    char* commandptr = NULL;

    if((commandptr = strchr(Asm->sheet.source + *been_read, ' ')) == NULL)
        return READING_ERROR;

    if((commandptr - (Asm->sheet.source + *been_read)) > COMMANDNAME_MAX)
        return SYNTAX_ERROR;

    ssize_t sizebuf = commandptr - (Asm->sheet.source + *been_read);
    strncpy(buffer, Asm->sheet.source + *been_read, sizebuf);
    buffer[sizebuf] = '\0';

    //fprintf(stderr, "buffer = %s\n", buffer);
    *been_read += commandptr - (Asm->sheet.source + *been_read) + 1; // +1 SO NEXT STRCHR DOESN'T INCLUDE LAST SEPARATION MARK
    //fprintf(stderr, " size of code = %zu\n", *been_read);
    return 0;
}


int CommandFind(char* buffer)
{
    for(size_t i = 1; i < COMMANDNAME_MAX; i++)
    {
        if(strcmp(buffer, CommandNames[i]) == 0)
            return i;
    }
    //fprintf(stderr,"Error in sintax\n");
    return 0;
}

size_t CodeResize(ASM_t* Asm, size_t oldcapacity)
{
    assert(Asm);
    static const int ReallocCoef = 2;
    size_t newcapacity = ReallocCoef * oldcapacity;

    Asm->code = (Code_t*)realloc(Asm->code, newcapacity * sizeof(Code_t)); assert(Asm->code);

    return newcapacity;
}

int  Convert_Code_To_Array(ASM_t* Asm)
{

    int size_of_code = 0;
    size_t been_read = 0;
    size_t capacity = default_max_code_size;

    while(been_read < Asm->sheet.size)
    {
        if ((size_of_code + 4) >= capacity)
        {
            //fprintf(stderr, "size = %lu\n", capacity);
            capacity = CodeResize(Asm, capacity); assert(Asm->code);
        }

        char buffer[COMMANDNAME_MAX] = {};

        if(GetCommand(Asm, buffer, &been_read) != 0)
        {
            fprintf(stderr, "Can't read command\n");
            return READING_ERROR;
        }

        char* lmarker = NULL;
        if((lmarker  = strchr(buffer, ':')) != NULL)
        {
            ArgLabel(Asm, buffer, size_of_code, lmarker);
            //fprintf(stderr, "buffer in loop =  %s\n", buffer);
            size_t buflen = strlen(buffer);
            //fprintf(stderr, "buflen = %lu\n", buflen);
            for(size_t i = 0; i < buflen; i++)
            {
                buffer[i] = '\0';
            }
            //fprintf(stderr, "buffer in loop =  %s\n", buffer);

            if(GetCommand(Asm, buffer, &been_read) != 0)
            {
                //fprintf(stderr, "Can't read command\n");
                return READING_ERROR;
            }
        }


        int command = CommandFind(buffer);

        if(command == PUSH)
        {
            Asm->code[size_of_code] = (Code_t)PUSH;
            size_of_code++;

            if(GetCommand(Asm, buffer, &been_read) != 0)
            {
                return READING_ERROR;
            }

            ArgPush(Asm, buffer, &size_of_code);
        }

        else if(command == JMP || command == JB || command ==  JA || command == JE ||
                command == JNE || command == JBE || command == JAE)
        {
            //fprintf(stderr, "jump code = %lg\n", Asm->code[size_of_code]);
            Asm->code[size_of_code] = (Code_t)command;
            //fprintf(stderr, "jump code = %lg\n", Asm->code[size_of_code]);
            size_of_code++;

            if(GetCommand(Asm, buffer, &been_read) != 0)
            {
                return READING_ERROR;
            }

            ArgJump(Asm, buffer, &size_of_code);
        }

        else if(command == POP)
        {
            Asm->code[size_of_code] = (Code_t)POP;
            size_of_code++;

            if(GetCommand(Asm, buffer, &been_read) != 0)
            {
                //fprintf(stderr, "Can't read command\n");
                return READING_ERROR;
            }

            ArgPop(Asm, buffer, &size_of_code);
        }

        else if(command == CALL)
        {
            //fprintf(stderr, "call code = %lg\n", Asm->code[size_of_code]);
            Asm->code[size_of_code] = (Code_t)CALL;
            //fprintf(stderr, "call code = %lg\n", Asm->code[size_of_code]);
            size_of_code++;

            if(GetCommand(Asm, buffer, &been_read) != 0)
            {
                return READING_ERROR;
            }

            ArgCall(Asm, buffer, &size_of_code);
        }

        else if(command != 0)
        {
            Asm->code[size_of_code] = (Code_t)command;
            size_of_code++;
        }
        else
        {
            size_of_code++;
        }
    }

    Asm->code_size = (uint32_t)size_of_code;
    //fprintf(stderr, "size = %d\n", Asm->code_size);

    return 0;
}
