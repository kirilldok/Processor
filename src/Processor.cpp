#include<stdio.h>
#include<stdbool.h>
#include<string.h>

#include"../MyStack/StackFunc.h"
#include"ProcessorFunc.h"
#include"Commands.h"


void RunCode(SPU_t* spu);
int Read_Prog_File(FILE* Input_code, SPU_t* spu, Header* hdr);
int HeaderRead(FILE* Input_code, Header* hdr);
FILE* Command_file_open(int argc, char** argv);
static Code_t UniPush(SPU_t* spu);
static Code_t* UniPop(SPU_t* spu);


int main(int argc, char* argv[])
{
    #ifndef NDEBUG
    FILE* clean_log = fopen("log.txt", "w+");
    fclose(clean_log);
    #endif

    FILE* clean_dump = fopen("SPU_Dump.txt", "w+");
    fclose(clean_dump);

    FILE* Input_code = NULL;
    Input_code = Command_file_open(argc, argv); assert(Input_code);

    SPU_t spu = { 0 };
    SpuCtor(&spu);

    Header hdr = { 0 };

    Read_Prog_File(Input_code, &spu, &hdr);
    fclose(Input_code);
    RunCode(&spu);

    SpuDtor(&spu);

    return 0;
}


int Read_Prog_File(FILE* Input_code, SPU_t* spu, Header* hdr)
{
    assert(Input_code); assert(spu); assert(hdr);

    if(HeaderRead(Input_code, hdr) != 0)
        return HEADER_READ_ERROR;

    spu->code_size = hdr->size;

    if (spu->code_size >= default_max_code_size)
    {
        //fprintf(stderr, "size = %lu\n", capacity);
        CodeResize(spu, spu->code_size); assert(spu->code);
    }

    fprintf(stderr, " size = %lu\n", spu->code_size * sizeof(size_t));
    fread(spu->code, sizeof(Code_t), spu->code_size, Input_code);
    // fprintf(stderr, "wr = %lu\n", wr);
    // fprintf(stderr, "size = %lu\n", sizeof(spu->code));
    //fprintf(stderr, "0 = %d\n", *spu->code);
    //fprintf(stderr, "read = %lu; size = %d\n", bufcount, spu->code_size);
    for(spu->ip = 0; spu->ip < hdr->size; spu->ip++)
    {
        //fscanf(Input_code, " %d", &spu->code[spu->ip]);
        fprintf(stderr, " %.x\n", (unsigned)spu->code[spu->ip]);
    }

    //while((fscanf(Input_code, " %d", &code[ip++])) && (code[ip-1] != 0));
    spu->ip = 0;

    SpuDump(spu);

    return NO_ERROR;
}



int HeaderRead(FILE* Input_code, Header* hdr)
{
    if(fread(&hdr->signature, sizeof(uint32_t), 1, Input_code) == 0)
        return READING_ERROR;

    if(fread(&hdr->version, sizeof(uint32_t), 1, Input_code) == 0)
        return READING_ERROR;

    if(fread(&hdr->size, sizeof(uint32_t), 1, Input_code) == 0)
        return READING_ERROR;

    if(fread(&hdr->reserved, sizeof(uint32_t), 1, Input_code) == 0)
        return READING_ERROR;

    return NO_ERROR;
}

FILE* Command_file_open(int argc, char** argv)
{
    FILE* code_file = NULL;

    if(argc > 1)
    {
        code_file  = fopen(argv[1], "r+b"); assert(code_file);
    }
    else
    {
        code_file = fopen("Machine_code.bin", "r+b"); assert(code_file);
    }
    return code_file;
}


void RunCode(SPU_t* spu)
{
    assert(spu);

    spu->ip = 0;
    bool Loop_flag = 1;

    while(Loop_flag)
    {
        switch((int)spu->code[spu->ip])
        {
            case PUSH:
            {
                ++spu->ip;
                Code_t result = UniPush(spu);
                StackPush(&spu->stk, result);
                ++spu->ip;
                SpuDump(spu);
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
                SpuDump(spu);
                break;
            }

            case POW:
            {
                Code_t arg_2 = 0;
                StackPop(&spu->stk, &arg_2);
                Code_t arg_1 = 0;
                StackPop(&spu->stk, &arg_1);

                for(int i; i < arg_1; i++)
                    arg_1 *= arg_1;

                StackPush(&spu->stk, arg_1);

                ++spu->ip;
                break;
            }

            case SQRT:
            {
                Code_t arg = 0;
                StackPop(&spu->stk, &arg);

                StackPush(&spu->stk, sqrt(arg));

                ++spu->ip;
                break;
            }

            case SIN:
            {
                Code_t arg = 0;
                StackPop(&spu->stk, &arg);

                StackPush(&spu->stk, sin(arg));

                ++spu->ip;
                break;
            }

            case IN:
            {
                Code_t arg = 0;
                fscanf(stdin, "%lg", &arg);

                StackPush(&spu->stk, arg);

                ++spu->ip;
                break;
            }

            case POP:
            {
                ++spu->ip;
                Code_t* result = UniPop(spu);
                StackPop(&spu->stk, result);
                SpuDump(spu);
                ++spu->ip;
                break;
            }

            case OUT:
            {
                Code_t arg = 0;
                StackPop(&spu->stk, &arg);
                fprintf(stderr, "Output element = '%lg' \n", arg);

                ++spu->ip;
                SpuDump(spu);
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

            case CALL:
            {
                StackPush(&spu->return_codes, (Code_t)(spu->ip + 2));
                spu->ip = (size_t)spu->code[spu->ip + 1];

                break;
            }

            case RET:
            {
                Code_t arg = 0;
                StackPop(&spu->return_codes, &arg);
                spu->ip = (size_t)arg;

                break;
            }

            case DUMP:
            {
                Stack_Dump(&spu->stk);
                ++spu->ip;

                break;
            }

            case HLT:
            {
                fprintf(stderr, "Programm finished with no errors.\n");
                Loop_flag = 0;
                //fclose(Prog_code);
                break;
            }

            default:
            {
                fprintf(stderr, "Sintax Error: '%.x; ip = %lu'\n", (unsigned)spu->code[spu->ip], spu->ip);
                SpuDump(spu);
                Loop_flag = 0;
            }

        }
    }

}



static Code_t UniPush(SPU_t* spu)
{
    //fprintf(stderr, "## VAL FOR IP = %d\n", spu->code[spu->ip]);
    //fprintf(stderr, "## VAL FOR IP + 1 = %d\n", spu->code[spu->ip]);
    uint32_t ArgCode = (uint32_t)spu->code[spu->ip];
    Code_t result = 0;

    if(ArgCode & REG_MASK)
    {
        //fprintf(stderr, "## IS REG\n");
        (spu->ip)++;
        result = spu->registers[(int)spu->code[spu->ip]];
    }

    if(ArgCode & C_MASK)
    {
        //fprintf(stderr, "## IS C\n");
        (spu->ip)++;
        result += spu->code[spu->ip];
    }

    if(ArgCode & RAM_MASK)
    {
        //fprintf(stderr, "## IS RAM\n");
        uint32_t addr = (uint32_t)result;
        result = spu->ram[addr];
    }

    return result; //TODO err codes
}


static Code_t* UniPop(SPU_t* spu)
{
    uint32_t ArgCode = (uint32_t)spu->code[spu->ip];

    Code_t* result = 0;
    int  tres = 0;

    if(ArgCode & REG_MASK)
    {
        (spu->ip)++;
        tres = (int)spu->code[spu->ip];
        result = &spu->registers[tres];
    }

    if(ArgCode & RAM_MASK)
    {
        if(ArgCode & REG_MASK && ArgCode & C_MASK)
        {
            (spu->ip)++;
            tres += (int)spu->code[spu->ip];
            result = &spu->ram[tres];
        }

        else if(ArgCode & C_MASK)
        {
            (spu->ip)++;
            tres = (int)spu->code[spu->ip];
            result = &spu->ram[tres];
        }

        else
        {
            (spu->ip)++;
            result = &spu->ram[tres];
        }
    }

    return result; //TODO err codes
}
