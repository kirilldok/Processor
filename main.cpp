#include<stdio.h>
#include<stdbool.h>
#include<string.h>

#include"StackFunc.h"
#include"Processor.h"


void RunCode(SPU_t* spu);
int Read_Prog_File(FILE* Input_code, SPU_t* spu);

int main(int argc, char* argv[])
{
    #ifndef NDEBUG
    FILE* clean = fopen("log.txt", "w+");
    fclose(clean);
    #endif

    SPU_t spu = { 0 };


    if (argc > 2)
    {
        FILE* input_file = fopen(argv[1], "r");
        Read_Prog_File(input_file, &spu);
        RunCode(&spu);
        //fclose(input_file);
    }
    else
    {
        FILE* default_file = fopen("Machine_code.txt", "r");
        fprintf(stderr, "No intput files. Default file Machine_code.txt opened.\n");
        Read_Prog_File(default_file, &spu);
        RunCode(&spu);
        //fclose(default_file);
    }

    return 0;
}


void RunCode(SPU_t* spu)//TODO struct stk->code[]
{

    Stack_t stk = {};
    StackCtor(&stk, 32);
    spu->ip = 0;

    bool Loop_flag = 1;

    while(Loop_flag)
    {
        switch(spu->code[spu->ip])
        {
            case PUSH:
            {
                StackPush(&stk, spu->code[++(spu->ip)]);
                ++spu->ip;
                break;
            }

            case SUB:
            {
                int arg_1 = 0;
                StackPop(&stk, &arg_1);
                int arg_2 = 0;
                StackPop(&stk, &arg_2);

                StackPush(&stk, arg_2  - arg_1);

                ++spu->ip;
                break;
            }

            case ADD:
            {
                int arg_1 = 0;
                StackPop(&stk, &arg_1);
                int arg_2 = 0;
                StackPop(&stk, &arg_2);

                StackPush(&stk, arg_2 + arg_1);

                ++spu->ip;
                break;
            }

            case DIV:
            {
                int arg_1 = 0;
                StackPop(&stk, &arg_1);
                int arg_2 = 0;
                StackPop(&stk, &arg_2);

                StackPush(&stk, arg_2 / arg_1);

                ++spu->ip;
                break;
            }

            case MUL:
            {
                int arg_1 = 0;
                StackPop(&stk, &arg_1);
                int arg_2 = 0;
                StackPop(&stk, &arg_2);

                StackPush(&stk, arg_1 * arg_2);

                ++spu->ip;
                break;
            }

            case OUT:
            {
                int arg = 0;
                StackPop(&stk, &arg);
                fprintf(stderr, "Output element = '%d' \n", arg);

                ++spu->ip;
                break;
            }

            case DUMP:
            {
                Stack_Dump(&stk);

                ++spu->ip;
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
                fprintf(stderr, "Sintax Error: '%d'\n", spu->code[spu->ip]);
                // fclose(Prog_code);
                Loop_flag = 0;
            }

        }
    }

}


int Read_Prog_File(FILE* Input_code, SPU_t* spu)
{
    fseek(Input_code, 13L , SEEK_SET);
    fscanf(Input_code, "%lu", &spu->code_size);
    //fprintf(stderr, "size = %lu\n", spu->code_size);

    for(spu->ip = 0; spu->ip < spu->code_size; spu->ip++)
    {
        fscanf(Input_code, " %d", &spu->code[spu->ip]);
        //fprintf(stderr, " %d\n", spu->code[spu->ip]);
    }

    //while((fscanf(Input_code, " %d", &code[ip++])) && (code[ip-1] != 0));
    spu->ip = 0;
    fclose(Input_code);

    return 1;
}

