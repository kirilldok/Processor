#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<stdint.h>

#include "PROGRAMM_ASM.h"


int main(int argc, const char* argv[])
{
    ASM_t Asm = { 0 };
    AsmCtor(&Asm);

    if (argc > 2)
    {
        FILE* CODE_ASM = fopen( argv[1], "r"); assert(CODE_ASM);
        Convert_Code_To_Array(CODE_ASM, &Asm);
    }
    else
    {
        FILE* default_f = fopen("default_programm_code.txt", "r"); assert(default_f);
        fprintf(stderr, "No intput files. Default file opened.\n");
        Convert_Code_To_Array(default_f, &Asm);
    }

    FILE* Machine_code = fopen("Machine_code.bin", "w+b"); assert(Machine_code);
    Write_in_file(Machine_code, &Asm);
    AsmDtor(&Asm);

    return 0;
}


int AsmCtor(ASM_t* Asm)
{
    assert(Asm);

    Asm->code = (char*)calloc(default_max_code_size, sizeof(char)); assert(Asm->code);

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

int Write_in_file(FILE* Output_code, ASM_t* Asm)
{
    assert(Output_code); assert(Asm);
    for(int i = 0; i < Asm->code_size; i++)
    {
        //fscanf(Input_code, " %d", &spu->code[spu->ip]);
        fprintf(stderr, " %d\n", Asm->code[i]);
    }

    uint32_t hdr[size_of_header] = {sign, ver, Asm->code_size, 0};

    fwrite(hdr, sizeof(hdr[0]), size_of_header, Output_code);
    fwrite(Asm->code, sizeof(char), Asm->code_size, Output_code);

    fclose(Output_code);
    fprintf(stderr, "code has been written to file\n");
    return 0;
}



int Register_convert(char str[])
{
    const char* REG_NAMES[10] =
    {
        "zx", "ax", "bx", "cx", "dx", "mlr"
    };

    if (strcmp(str, REG_NAMES[ZX]) == 0)
        return ZX;

    else if (strcmp(str, REG_NAMES[AX]) == 0)
        return AX;

    else if (strcmp(str, REG_NAMES[BX]) == 0)
        return BX;

    else if (strcmp(str, REG_NAMES[CX]) == 0)
        return CX;

    else if (strcmp(str, REG_NAMES[DX]) == 0)
        return DX;

    else if (strcmp(str, REG_NAMES[MLR]) == 0)
        return MLR;

    else
    {
        //fprintf(stderr,"Error in register sintax\n");
        return -1;
    }
}


size_t GetFileSize(FILE* file)
{

    fseek(file, 0L, SEEK_END);

    size_t size = ftell(file);

    rewind(file);
    return size;
}


char CommandFind(char* buffer)
{
    if(strcmp(buffer, CommandNames[PUSH]) == 0)
    {
        return PUSH;
    }
    else if(strcmp(buffer, CommandNames[POP]) == 0)
    {
        return POP;
    }
    else if(strcmp(buffer, CommandNames[ADD]) == 0)
    {
        return ADD;
    }
    else if(strcmp(buffer, CommandNames[SUB]) == 0)
    {
        return SUB;
    }
    else if(strcmp(buffer, CommandNames[DIV]) == 0)
    {
        return DIV;
    }
    else if(strcmp(buffer, CommandNames[MUL]) == 0)
    {
        return MUL;
    }
    else if(strcmp(buffer, CommandNames[POW]) == 0)
    {
        return POW;
    }
    else if(strcmp(buffer, CommandNames[SQRT]) == 0)
    {
        return SQRT;
    }
    else if(strcmp(buffer, CommandNames[SIN]) == 0)
    {
        return SIN;
    }
    else if(strcmp(buffer, CommandNames[OUT]) == 0)
    {
        return OUT;
    }
    else if(strcmp(buffer, CommandNames[JMP]) == 0)
    {
        return JMP;
    }
    else if(strcmp(buffer, CommandNames[JBE]) == 0)
    {
        return JBE;
    }
    else if(strcmp(buffer, CommandNames[JAE]) == 0)
    {
        return JAE;
    }
    else if(strcmp(buffer, CommandNames[JA]) == 0)
    {
        return JA;
    }
    else if(strcmp(buffer, CommandNames[JB]) == 0)
    {
        return JB;
    }
    else if(strcmp(buffer, CommandNames[JNE]) == 0)
    {
        return JNE;
    }
    else if(strcmp(buffer, CommandNames[JE]) == 0)
    {
        return JE;
    }
    else if(strcmp(buffer, CommandNames[DUMP]) == 0)
    {
        return DUMP;
    }
    else if(strcmp(buffer, CommandNames[31]) == 0)
    {
        return HLT;
    }
    else
        return 0;
}

int  Convert_Code_To_Array(FILE* Input_code, ASM_t* Asm)
{
    size_t size_of_file = GetFileSize(Input_code);
    int size_of_code = 0;

    while(ftell(Input_code) < size_of_file)
    {
        char buffer[COMMANDNAME_MAX] = {};
        fscanf(Input_code, "%s", buffer);

        char* lmarker = NULL;
        if((lmarker  = strchr(buffer, ':')) != NULL)
        {
            ArgLabel(Asm, lmarker, buffer, size_of_code);
            fscanf(Input_code, "%s", buffer); assert(buffer);
        }


        char command = CommandFind(buffer);

        if(command == PUSH)
        {
            Asm->code[size_of_code] = PUSH;
            size_of_code++;
            fscanf(Input_code, "%s", buffer); assert(buffer);
            ArgPush(Asm, buffer, &size_of_code);
        }
        else if(command == JMP || command == JB || command ==  JA || command == JE ||
                command == JNE || command == JBE || command == JAE)
        {
            Asm->code[size_of_code] = command;
            fscanf(Input_code, "%s", buffer); assert(buffer);
            ArgJump(Asm, buffer, &size_of_code);
        }
        else if(command == POP)
        {
            Asm->code[size_of_code] = POP;
            size_of_code++;
            fscanf(Input_code, "%s", buffer); assert(buffer);
            ArgPop(Asm, buffer, &size_of_code);
        }
        else if(command != 0)
        {
            Asm->code[size_of_code] = command;
            size_of_code++;
        }
        else
            size_of_code++;
    }

    Asm->code_size = size_of_code;
    //fprintf(stderr, "size = %d\n", Asm->code_size);

    return 0;
}


        //fprintf(Output_code, "%d", Asm->code[i]);
//         switch(Asm->code[i])
//         {
//             case PUSH:
//             {
//                 i++;
//                 fprintf(Output_code, " %d\n", Asm->code[i]);
//                 break;
//             }
//
//             case JMP:
//             {
//                 i++;
//                 fprintf(Output_code, " %d\n", Asm->code[i]);
//                 break;
//             }
//
//             case JA:
//             {
//                 i++;
//                 fprintf(Output_code, " %d\n", Asm->code[i]);
//                 break;
//             }
//
//             case JAE:
//             {
//                 i++;
//                 fprintf(Output_code, " %d\n", Asm->code[i]);
//                 break;
//             }
//
//             case JB:
//             {
//                 i++;
//                 fprintf(Output_code, " %d\n", Asm->code[i]);
//                 break;
//             }
//
//             case JBE:
//             {
//                 i++;
//                 fprintf(Output_code, " %d\n", Asm->code[i]);
//                 break;
//             }
//
//             case JE:
//             {
//                 i++;
//                 fprintf(Output_code, " %d\n", Asm->code[i]);
//                 break;
//             }
//
//             case JNE:
//             {
//                 i++;
//                 fprintf(Output_code, " %d\n", Asm->code[i]);
//                 break;
//             }
//
//             case PUSHR:
//             {
//                 i++;
//                 fprintf(Output_code, " %d\n", Asm->code[i]);
//                 break;
//             }
//
//             case POP:
//             {
//                 i++;
//                 fprintf(Output_code, " %d\n", Asm->code[i]);
//                 break;
//             }
//
//
//
//             default:
//                 fprintf(Output_code, "\n");
//         }
