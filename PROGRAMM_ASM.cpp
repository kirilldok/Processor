#include<stdio.h>
#include<stdbool.h>
#include<string.h>

#include "StackFunc.h"
#include "Processor.h"


int Convert_Code_To_Array(FILE* Input_code, SPU_t* code);
int Write_in_file(FILE* Output_code, SPU_t* spu);

int main(int argc, const char* argv[])
{
    SPU_t spu = { 0 };

    if (argc > 2)
    {
        FILE* CODE_ASM = fopen( argv[1], "r");
        Convert_Code_To_Array(CODE_ASM, &spu);
    }
    else
    {
        FILE* default_f = fopen("default_programm_code.txt", "r");
        fprintf(stderr, "No intput files. Default file opened.\n");
        Convert_Code_To_Array(default_f, &spu);
    }

    FILE* Machine_code = fopen("Machine_code.txt", "w+");
    Write_in_file(Machine_code, &spu);

    return 0;
}



int  Convert_Code_To_Array(FILE* Input_code, SPU_t* spu)
{
    size_t size_of_code = 0;
    bool Loop_flag = 1;

    while(Loop_flag)
    {
        char command[50] = " ";
        fscanf(Input_code, "%s", command);
        if (strcmp(command, "push") == 0)
        {
            spu->code[size_of_code] = PUSH;
            size_of_code++;

            Code_t arg = 0;
            fscanf(Input_code, "%d", &arg);
            spu->code[size_of_code] = arg;
            size_of_code++;
        }

        else if (strcmp(command, "sub") == 0)
        {
            spu->code[size_of_code] = SUB;
            size_of_code++;
        }

        else if(strcmp(command, "add") == 0)
        {
            spu->code[size_of_code] = ADD;
            size_of_code++;
        }

        else if(strcmp(command, "div") == 0)
        {
            spu->code[size_of_code] = DIV;
            size_of_code++;
        }

        else if(strcmp(command, "mul") == 0)
        {
            spu->code[size_of_code] = MUL;
            size_of_code++;
        }

        else if(strcmp(command, "out") == 0)
        {
            spu->code[size_of_code] = OUT;
            size_of_code++;
        }

        else if(strcmp(command, "dump") == 0)
        {
            spu->code[size_of_code] = DUMP;
            size_of_code++;
        }


        else if(strcmp(command, "jmp") == 0)
        {
            spu->code[size_of_code] = JMP;
            size_of_code++;

            int arg = 0;
            fscanf(Input_code, "%d", &arg);
            spu->code[size_of_code] = arg;
            size_of_code++;
        }

        else if(strcmp(command, "ja") == 0)
        {
            spu->code[size_of_code] = JA;
            size_of_code++;

            Code_t arg = 0;
            fscanf(Input_code, "%d", &arg);
            spu->code[size_of_code] = arg;
            size_of_code++;
        }

        else if(strcmp(command, "jae") == 0)
        {
            spu->code[size_of_code] = JAE;
            size_of_code++;

            Code_t arg = 0;
            fscanf(Input_code, "%d", &arg);
            spu->code[size_of_code] = arg;
            size_of_code++;
        }

        else if(strcmp(command, "jb") == 0)
        {
            spu->code[size_of_code] = JB;
            size_of_code++;

            Code_t arg = 0;
            fscanf(Input_code, "%d", &arg);
            spu->code[size_of_code] = arg;
            size_of_code++;
        }

        else if(strcmp(command, "jbe") == 0)
        {
            spu->code[size_of_code] = JBE;
            size_of_code++;

            Code_t arg = 0;
            fscanf(Input_code, "%d", &arg);
            spu->code[size_of_code] = arg;
            size_of_code++;
        }

        else if(strcmp(command, "je") == 0)
        {
            spu->code[size_of_code] = JE;
            size_of_code++;

            Code_t arg = 0;
            fscanf(Input_code, "%d", &arg);
            spu->code[size_of_code] = arg;
            size_of_code++;
        }

        else if(strcmp(command, "jne") == 0)
        {
            spu->code[size_of_code] = JNE;
            size_of_code++;

            Code_t arg = 0;
            fscanf(Input_code, "%d", &arg);
            spu->code[size_of_code] = arg;
            size_of_code++;
        }


        else if(strcmp(command, "pop") == 0)
        {
            spu->code[size_of_code] = PUSHR;
            size_of_code++;

            int arg = 0;
            fscanf(Input_code, "%d", &arg);
            spu->code[size_of_code] = arg;
            size_of_code++;
        }

        else if(strcmp(command, "pushr") == 0)
        {
            spu->code[size_of_code] = PUSHR;
            size_of_code++;

            int arg = 0;
            fscanf(Input_code, "%d", &arg);
            spu->code[size_of_code] = arg;
            size_of_code++;
        }

        else if(strcmp(command, "hlt") == 0)
        {
            spu->code[size_of_code] = HLT;
            size_of_code++;
            fclose(Input_code);
            spu->code_size = size_of_code;
            Loop_flag = 0;

            return 1;
        }

        else
        {
            fclose(Input_code);
            fprintf(stderr, "Sintax Error: '%s'\n", command);
            Loop_flag = 0;

            return 0;
        }
    }

    return 0;
}




int Write_in_file(FILE* Output_code, SPU_t* spu)
{
    fprintf(Output_code, "955181419\n");
    fprintf(Output_code, "2.0\n");
    fprintf(Output_code, "%lu\n", spu->code_size);

    for(size_t i = 0; i < spu->code_size; i++)
    {
        fprintf(Output_code, "%d", spu->code[i]);
        switch(spu->code[i])
        {
            case PUSH:
            {
                i++;
                fprintf(Output_code, " %d\n", spu->code[i]);
                break;
            }

            case JMP:
            {
                i++;
                fprintf(Output_code, " %d\n", spu->code[i]);
                break;
            }

            case JA:
            {
                i++;
                fprintf(Output_code, " %d\n", spu->code[i]);
                break;
            }

            case JAE:
            {
                i++;
                fprintf(Output_code, " %d\n", spu->code[i]);
                break;
            }

            case JB:
            {
                i++;
                fprintf(Output_code, " %d\n", spu->code[i]);
                break;
            }

            case JBE:
            {
                i++;
                fprintf(Output_code, " %d\n", spu->code[i]);
                break;
            }

            case JE:
            {
                i++;
                fprintf(Output_code, " %d\n", spu->code[i]);
                break;
            }

            case JNE:
            {
                i++;
                fprintf(Output_code, " %d\n", spu->code[i]);
                break;
            }

            case PUSHR:
            {
                i++;
                fprintf(Output_code, " %d\n", spu->code[i]);
                break;
            }

            case POP:
            {
                i++;
                fprintf(Output_code, " %d\n", spu->code[i]);
                break;
            }



            default:
                fprintf(Output_code, "\n");
        }
    }

    fclose(Output_code);
    fprintf(stderr, "code has been written to file\n");
    return 0;
}
