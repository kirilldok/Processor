#include<stdio.h>
#include<stdbool.h>
#include <string.h>
#include "StackFunc.h"

void RunCode();

int main(int argc, char* argv[])
{
    #ifndef NDEBUG
    FILE* clean = fopen("log.txt", "w+b");
    fclose(clean);
    #endif

    RunCode();

    return 0;

}


void RunCode()
{
    Stack_t stk = {};
    StackCtor(&stk, 32);

    bool Loop_flag = 1;

    while(Loop_flag)
    {
        char command[50] = " ";
        scanf("%s", command);
        if (strcmp(command, "push") == 0)
        {
            int arg = 0;
            scanf("%d", &arg);
            StackPush(&stk, arg);
        }

        else if (strcmp(command, "sub") == 0)
        {
            int arg_1 = 0;
            StackPop(&stk, &arg_1);
            int arg_2 = 0;
            StackPop(&stk, &arg_2);

            StackPush(&stk, arg_2  - arg_1);
        }

        else if(strcmp(command, "add") == 0)
        {
            int arg_1 = 0;
            StackPop(&stk, &arg_1);
            int arg_2 = 0;
            StackPop(&stk, &arg_2);

            StackPush(&stk, arg_2 + arg_1);
        }

        else if(strcmp(command, "div") == 0)
        {
            int arg_1 = 0;
            StackPop(&stk, &arg_1);
            int arg_2 = 0;
            StackPop(&stk, &arg_2);

            StackPush(&stk, arg_2 / arg_1);
        }

        else if(strcmp(command, "mult") == 0)
        {
            int arg_1 = 0;
            StackPop(&stk, &arg_1);
            int arg_2 = 0;
            StackPop(&stk, &arg_2);

            StackPush(&stk, arg_1 * arg_2);
        }

        else if(strcmp(command, "out") == 0)
        {
            int arg = 0;
            StackPop(&stk, &arg);
            printf("Output element = '%d' \n", arg);
        }

        else if(strcmp(command, "dump") == 0)
        {
            Stack_Dump(&stk);
        }

        else if(strcmp(command, "hlt") == 0)
        {
            StackDtor(&stk);
            fprintf(stderr, "Programm finished with no errors.\n");
            Loop_flag = 0;
            break;
        }

        else
        {
            StackDtor(&stk);
            fprintf(stderr, "Sintax Error: '%s'\n", command);
            Loop_flag = 0;
        }
    }
}

