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
    FILE* clean_log = fopen("log.txt", "w+");
    fclose(clean_log);
    #endif

    FILE* clean_dump = fopen("SPU_Dump.txt", "w+b");
    fclose(clean_dump);


    SPU_t spu = { };


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


void RunCode(SPU_t* spu)
{
    StackCtor(&spu->stk, 2);


    spu->ip = 0;

    bool Loop_flag = 1;

    while(Loop_flag)
    {
        switch(spu->code[spu->ip])
        {
            case PUSH:
            {
                StackPush(&spu->stk, spu->code[++(spu->ip)]);
                ++spu->ip;

                //SpuDump(spu);
                break;
            }

            case SUB:
            {
                Code_t arg_1 = 0;
                StackPop(&spu->stk, &arg_1);
                Code_t arg_2 = 0;
                StackPop(&spu->stk, &arg_2);

                StackPush(&spu->stk, arg_2  - arg_1);

                ++spu->ip;
                break;
            }

            case ADD:
            {
                Code_t arg_1 = 0;
                StackPop(&spu->stk, &arg_1);
                Code_t arg_2 = 0;
                StackPop(&spu->stk, &arg_2);

                StackPush(&spu->stk, arg_2 + arg_1);

                ++spu->ip;
                //SpuDump(spu);

                break;
            }

            case DIV:
            {
                Code_t arg_1 = 0;
                StackPop(&spu->stk, &arg_1);
                Code_t arg_2 = 0;
                StackPop(&spu->stk, &arg_2);

                StackPush(&spu->stk, arg_2 / arg_1);

                ++spu->ip;
                break;
            }

            case MUL:
            {
                Code_t arg_1 = 0;
                StackPop(&spu->stk, &arg_1);
                Code_t arg_2 = 0;
                StackPop(&spu->stk, &arg_2);

                StackPush(&spu->stk, arg_1 * arg_2);

                ++spu->ip;
                //SpuDump(spu);
                break;
            }

            case OUT:
            {
                Code_t arg = 0;
                StackPop(&spu->stk, &arg);
                fprintf(stderr, "Output element = '%d' \n", arg);

                ++spu->ip;
                //SpuDump(spu);
                break;
            }

            case DUMP:
            {
                Stack_Dump(&spu->stk);

                ++spu->ip;
                break;
            }


            case JMP:
            {
                spu->ip = (size_t)spu->code[spu->ip + 1];

                break;
            }

            case JA:
            {
                Code_t arg_2 = 0;
                StackPop(&spu->stk, &arg_2);
                Code_t arg_1 = 0;
                StackPop(&spu->stk, &arg_2);

                if(arg_1 > arg_2)
                    spu->ip = (size_t)spu->code[spu->ip + 1];
                else
                    spu->ip += 2;

                break;
            }

            case JAE:
            {
                Code_t arg_2 = 0;
                StackPop(&spu->stk, &arg_2);
                Code_t arg_1 = 0;
                StackPop(&spu->stk, &arg_1);

                if(arg_1 >= arg_2)
                    spu->ip = (size_t)spu->code[spu->ip + 1];
                else
                    spu->ip += 2;

                break;
            }

            case JB:
            {
                Code_t arg_2 = 0;
                StackPop(&spu->stk, &arg_2);
                Code_t arg_1 = 0;
                StackPop(&spu->stk, &arg_1);

                if(arg_1 < arg_2)
                    spu->ip = (size_t)spu->code[spu->ip + 1];
                else
                    spu->ip += 2;
                //SpuDump(spu);
                break;
            }

            case JBE:
            {
                Code_t arg_2 = 0;
                StackPop(&spu->stk, &arg_2);
                Code_t arg_1 = 0;
                StackPop(&spu->stk, &arg_1);

                if(arg_1 <= arg_2)
                    spu->ip = (size_t)spu->code[spu->ip + 1];
                else
                    spu->ip += 2;

                break;
            }

            case JE:
            {
                Code_t arg_1 = 0;
                StackPop(&spu->stk, &arg_1);
                Code_t arg_2 = 0;
                StackPop(&spu->stk, &arg_2);

                if(arg_2 == arg_1)
                    spu->ip = (size_t)spu->code[spu->ip + 1];
                else
                    spu->ip += 2;

                break;
            }

            case JNE:
            {
                Code_t arg_1 = 0;
                StackPop(&spu->stk, &arg_1);
                Code_t arg_2 = 0;
                StackPop(&spu->stk, &arg_2);

                if(arg_2 != arg_1)
                    spu->ip = (size_t)spu->code[spu->ip + 1];
                else
                    spu->ip += 2;

                break;
            }

            case POP:
            {
                Code_t arg = 0;
                StackPop(&spu->stk, &arg);

                spu->registers[spu->code[++(spu->ip)]] = arg;
                ++spu->ip;
                //SpuDump(spu);
                break;
            }

            case PUSHR:
            {
                StackPush(&spu->stk, spu->registers[spu->code[++(spu->ip)]]);
                ++spu->ip;
                //SpuDump(spu);
                break;
            }

            case HLT:
            {
                StackDtor(&spu->stk);
                fprintf(stderr, "Programm finished with no errors.\n");
                Loop_flag = 0;
                //fclose(Prog_code);
                break;
            }

            default:
            {
                StackDtor(&spu->stk);
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

    SpuDump(spu);
    fclose(Input_code);

    return 1;
}

