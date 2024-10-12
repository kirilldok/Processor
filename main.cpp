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


void RunCode(int code[]);
int Read_Prog_File(FILE* Input_code, int code[]);

int main(int argc, char* argv[])
{
    #ifndef NDEBUG
    FILE* clean = fopen("log.txt", "w+");
    fclose(clean);
    #endif

    int code[128] = { 0 };

    if (argc > 2)
    {
        FILE* input_file = fopen(argv[1], "r");
        Read_Prog_File(input_file, code);
        RunCode(code);
        //fclose(input_file);
    }
    else
    {
        FILE* default_file = fopen("Machine_code.txt", "r");
        fprintf(stderr, "No intput files. Default file Machine_code.txt opened.\n");
        Read_Prog_File(default_file, code);
        RunCode(code);
        //fclose(default_file);
    }

    return 0;
}


void RunCode(int code[])
{

    Stack_t stk = {};
    StackCtor(&stk, 32);

    bool Loop_flag = 1;
    int ip = 0;

    while(Loop_flag)
    {
        switch(code[ip])
        {
            case PUSH:
            {
                StackPush(&stk, code[++ip]);
                ++ip;
                break;
            }

            case SUB:
            {
                int arg_1 = 0;
                StackPop(&stk, &arg_1);
                int arg_2 = 0;
                StackPop(&stk, &arg_2);

                StackPush(&stk, arg_2  - arg_1);

                ++ip;
                break;
            }

            case ADD:
            {
                int arg_1 = 0;
                StackPop(&stk, &arg_1);
                int arg_2 = 0;
                StackPop(&stk, &arg_2);

                StackPush(&stk, arg_2 + arg_1);

                ++ip;
                break;
            }

            case DIV:
            {
                int arg_1 = 0;
                StackPop(&stk, &arg_1);
                int arg_2 = 0;
                StackPop(&stk, &arg_2);

                StackPush(&stk, arg_2 / arg_1);

                ++ip;
                break;
            }

            case MUL:
            {
                int arg_1 = 0;
                StackPop(&stk, &arg_1);
                int arg_2 = 0;
                StackPop(&stk, &arg_2);

                StackPush(&stk, arg_1 * arg_2);

                ++ip;
                break;
            }

            case OUT:
            {
                int arg = 0;
                StackPop(&stk, &arg);
                fprintf(stderr, "Output element = '%d' \n", arg);

                ++ip;
                break;
            }

            case DUMP:
            {
                Stack_Dump(&stk);

                ++ip;
                break;
            }

            case HLT:
            {
                StackDtor(&stk);
                fprintf(stderr, "Programm finished with no errors.\n");
                Loop_flag = 0;
                //fclose(Prog_code);
                break;
            }

            default:
            {
                StackDtor(&stk);
                fprintf(stderr, "Sintax Error: '%d'\n", code[ip]);
                //fclose(Prog_code);
                Loop_flag = 0;
            }

        }
    }

}

 
int Read_Prog_File(FILE* Input_code, int code[])
{
    int ip = 0;

    while((fscanf(Input_code, " %d", &code[ip++])) && (code[ip-1] != 0));

    fclose(Input_code);

    return 1;
}

