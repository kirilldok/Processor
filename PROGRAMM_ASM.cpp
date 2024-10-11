#include<stdio.h>
#include<stdbool.h>
#include <string.h>
#include "StackFunc.h"

enum Machine_Commands
{
    PUSH = 1,
    OUT  = 10,
    ADD  = 11,
    SUB  = 100,
    MUL  = 101,
    DIV  = 110,
    DUMP = 111,

    HLT  = 0,
};

int ConvertCode(FILE* Input_code, FILE* Output_code);

int main(int argc, const char* argv[])
{
    FILE* Machine_code = fopen("Machine_code.txt", "w+");
    int size_of_code = 0;

    if (argc > 2)
    {
        FILE* CODE_ASM = fopen( argv[1], "r");
        size_of_code = ConvertCode(CODE_ASM, Machine_code);
    }
    else
    {
        FILE* default_f = fopen("default_programm_code.txt", "r");
        fprintf(stderr, "No intput files. Default file opened.\n");
        size_of_code = ConvertCode(default_f,  Machine_code);
    }

    printf("size of code = %d\n", size_of_code);


}



int  ConvertCode(FILE* Input_code,  FILE* Output_code)
{
    int size_of_code;
    bool Loop_flag = 1;


    while(Loop_flag)
    {
        char command[50] = " ";
        fscanf(Input_code, "%s", command);
        if (strcmp(command, "push") == 0)
        {
            int arg = 0;
            fscanf(Input_code, "%d", &arg);
            fprintf(Output_code, "%d %d\n", PUSH, arg);
            size_of_code += 2;
        }

        else if (strcmp(command, "sub") == 0)
        {
            fprintf(Output_code, "%d\n", SUB);
            size_of_code++;
        }

        else if(strcmp(command, "add") == 0)
        {
            fprintf(Output_code, "%d\n", ADD);
            size_of_code++;
        }

        else if(strcmp(command, "div") == 0)
        {
            fprintf(Output_code, "%d\n", DIV);
            size_of_code++;
        }

        else if(strcmp(command, "mul") == 0)
        {
            fprintf(Output_code, "%d\n", MUL);
            size_of_code++;
        }

        else if(strcmp(command, "out") == 0)
        {
            fprintf(Output_code, "%d\n", OUT);
            size_of_code++;
        }

        else if(strcmp(command, "dump") == 0)
        {
            fprintf(Output_code, "%d\n", DUMP);
            size_of_code++;
        }

        else if(strcmp(command, "hlt") == 0)
        {
            fprintf(Output_code, "%d\n", HLT);
            size_of_code++;
            fclose(Output_code);
            fclose(Input_code);
            Loop_flag = 0;

            return size_of_code;
        }

        else
        {
            fclose(Output_code);
            fclose(Input_code);
            fprintf(stderr, "Sintax Error: '%s'\n", command);
            Loop_flag = 0;

            return 0;
        }
    }

    return 0;
}
