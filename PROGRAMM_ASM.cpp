#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<stdint.h>

#include "Processor.h"
#include "Commands.h"
#include "PROGRAMM_ASM.h"


int main(int argc, const char* argv[])
{
    SPU_t spu = { 0 };
    SpuCtor(&spu);

    if (argc > 2)
    {
        FILE* CODE_ASM = fopen( argv[1], "r"); assert(CODE_ASM);
        Convert_Code_To_Array(CODE_ASM, &spu);
    }
    else
    {
        FILE* default_f = fopen("default_programm_code.txt", "r"); assert(default_f);
        fprintf(stderr, "No intput files. Default file opened.\n");
        Convert_Code_To_Array(default_f, &spu);
    }

    FILE* Machine_code = fopen("Machine_code.bin", "w+b"); assert(Machine_code);
    Write_in_file(Machine_code, &spu);
    SpuDtor(&spu);

    return 0;
}


int Write_in_file(FILE* Output_code, SPU_t* spu)
{
    assert(Output_code); assert(spu);
    uint32_t hdr[size_of_header] = {sign, ver, spu->code_size, 0};

    fwrite(hdr, sizeof(hdr[0]), size_of_header, Output_code);
    fwrite(spu->code, sizeof(spu->code), spu->code_size, Output_code);

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
        fprintf(stderr,"Error in register sintax\n");
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


int CommandFind(char* buffer)
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

int  Convert_Code_To_Array(FILE* Input_code, SPU_t* spu)
{
    size_t size_of_file = GetFileSize(Input_code);
    unsigned size_of_code = 0;

    while(ftell(Input_code) < size_of_file)
    {
        char buffer[COMMANDNAME_MAX] = {};
        fscanf(Input_code, "%s", buffer);

        int command = CommandFind(buffer);

        if(command == PUSH)
        {
            spu->code[size_of_code] = PUSH;
            size_of_code++;

            if(fscanf(Input_code, "%s", buffer) == 0)
            {
                return READING_ERROR;
            }

            AsmPush(spu, buffer, &size_of_code);
        }

        else if(command == JMP || command == JB || command ==  JA || command == JE ||
                command == JNE || command == JBE || command == JAE)
        {
            spu->code[size_of_code] = command;
            if(fscanf(Input_code, "%s", buffer) == 0)
            {
                return READING_ERROR;
            }

            AsmJump(spu, buffer, &size_of_code);

        }


    }

    return 0;
}



int AsmPush(Spu_t* spu, char* buffer, int* size_of_code)
{

    char* cptr = NULL; // pointer of begin [
    char* eptr = NULL; // pointer of end ]
    char* plusptr = NULL; // pointer of +
    char* regArg = NULL;
    char argBuffer[ARGLEN_MAX] = {};
    uint32_t reg = 0;

    if((cptr = strchr(buffer, '[')) != NULL)
    {
        if((plusptr = strchr(buffer, '+')) != NULL)
        {
            spu->code[*size_of_code] = RAM_REG_CONSTVAL;
            (*size_of_code)++;

            //fprintf(stderr, "## ARGTYPE = %d\n", RAM_REG_CONSTVAL);

            if((reg = Register_convert(buffer + 1)) == -1) // buffer + (cptr - buffer)
            {
                fprintf(stderr, "SYNTAX ERROR\n");
                return SYNTAX_ERROR;
            }

            spu->code[*size_of_code] = reg;
            (*size_of_code)++;

            if((eptr = strchr(buffer, ']')) == NULL)
            {
                fprintf(stderr, "SYNTAX ERROR\n");
                return SYNTAX_ERROR;
            }


            spu->code[*size_of_code] = atoi(buffer + (eptr - plusptr + 1));
            (*size_of_code)++;
        }

        else if((rg = Register_convert(buffer + 1)) != -1) // buffer + (cptr - buffer)
        {
            spu->code[*size_of_code] = RAM_REG;
            (*size_of_code)++;

            //ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_REG));

            spu->code[*size_of_code] = reg;
            (*size_of_code)++;
        }
        else
        {
            spu->code[*size_of_code] = RAM_CONSTVAL;
            (*size_of_code)++;

            //ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_CONSTVAL));

            spu->code[*size_of_code] = atoi(buffer + 1);
            (*size_of_code)++;
        }
    }
    else if((plusptr = strchr(buffer, '+')) != NULL)
    {
        spu->code[*size_of_code] = REG_CONSTVAL;
        (*size_of_code)++;

        ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", REG_CONSTVAL));

        if((reg = Register_convert(buffer)) == -1)
        {
            fprintf(stderr, "SYNTAX ERROR\n");
            return SYNTAX_ERROR;
        }

        spu->code[*size_of_code] = reg;
        (*size_of_code)++;

        spu->code[*size_of_code] = atoi(buffer + (plusptr - buffer) + 1);
        (*size_of_code)++;

    }
    else if((reg = Register_convert(buffer)) != -1)
    {
        spu->code[*size_of_code] = REG;
        (*size_of_code)++;

        //ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", REG));

        spu->code[*size_of_code] = reg;
        (*size_of_code)++;
    }
    else
    {
        spu->code[*size_of_code] = CONSTVAL;
        (*size_of_code)++;
        spu_code[*size_of_code] = atoi(buffer);
        (*size_of_code)++;

        //ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", CONSTVAL));
    }
    return 0;
}


int AsmJump(SPU_t* spu, char* buffer, int* size_of_code)
{
    //fprintf(stderr, "## JMP ARG: %s\n", buffer));

    if(isdigit(buffer[0]))
    {
        spu->code[*size_of_code + 1] = atoi(buffer);
        //fprintf(stderr, "## JMP ARG ADDED: %d\n", atoi(buffer)));
        *size_of_code += 2;                        // JUMP CODE AND ITS ARGUMENT
        return 0;
    }

    else
    {
        int labVal = 0;

        if((labVal = FindLabel(&Asm->LTable, buffer)) == -1) // LABEL IS NOT IN THE TABLE YET
        {
            strncpy(Asm->LTable.labAr[Asm->LTable.lnum].name, buffer, COMMANDNAME_MAX);
            Asm->LTable.lnum++;
            *cycleIndex += 2;   // JUMP CODE AND ITS ARGUMENT
            ON_DEBUG(fprintf(stderr, "## LABEL NAME ADDED: %s\n", buffer));
            ON_DEBUG(LTDump(&Asm->LTable));
            return 0;
        }

        else
        {
            spu->code[*size_of_code + 1] = spu->LTable.labAr[labVal].ipTarg;
            *cycleIndex += 2;                    // JUMP CODE AND ITS ARGUMENT
            return 0;
        }
    }
}


        //fprintf(Output_code, "%d", spu->code[i]);
//         switch(spu->code[i])
//         {
//             case PUSH:
//             {
//                 i++;
//                 fprintf(Output_code, " %d\n", spu->code[i]);
//                 break;
//             }
//
//             case JMP:
//             {
//                 i++;
//                 fprintf(Output_code, " %d\n", spu->code[i]);
//                 break;
//             }
//
//             case JA:
//             {
//                 i++;
//                 fprintf(Output_code, " %d\n", spu->code[i]);
//                 break;
//             }
//
//             case JAE:
//             {
//                 i++;
//                 fprintf(Output_code, " %d\n", spu->code[i]);
//                 break;
//             }
//
//             case JB:
//             {
//                 i++;
//                 fprintf(Output_code, " %d\n", spu->code[i]);
//                 break;
//             }
//
//             case JBE:
//             {
//                 i++;
//                 fprintf(Output_code, " %d\n", spu->code[i]);
//                 break;
//             }
//
//             case JE:
//             {
//                 i++;
//                 fprintf(Output_code, " %d\n", spu->code[i]);
//                 break;
//             }
//
//             case JNE:
//             {
//                 i++;
//                 fprintf(Output_code, " %d\n", spu->code[i]);
//                 break;
//             }
//
//             case PUSHR:
//             {
//                 i++;
//                 fprintf(Output_code, " %d\n", spu->code[i]);
//                 break;
//             }
//
//             case POP:
//             {
//                 i++;
//                 fprintf(Output_code, " %d\n", spu->code[i]);
//                 break;
//             }
//
//
//
//             default:
//                 fprintf(Output_code, "\n");
//         }
