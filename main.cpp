#include<stdio.h>
#include<stdbool.h>
#include<string.h>

#include"StackFunc.h"
#include"Processor.h"
#include"Commands.h"


void RunCode(SPU_t* spu);
int Read_Prog_File(FILE* Input_code, SPU_t* spu, Header* hdr);
int HeaderRead(FILE* Input_code, Header* hdr);
FILE* Command_file_open(int argc, char** argv);
static int UniPush(SPU_t* spu);
static int UniPop(SPU_t* spu);


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

    size_t bufcount = fread(spu->code, sizeof(char), spu->code_size, Input_code);
    // fprintf(stderr, "wr = %lu\n", wr);
    // fprintf(stderr, "size = %lu\n", sizeof(spu->code));
    //fprintf(stderr, "0 = %d\n", *spu->code);
    //fprintf(stderr, "read = %lu; size = %d\n", bufcount, spu->code_size);
    for(spu->ip = 0; spu->ip < hdr->size; spu->ip++)
    {
        //fscanf(Input_code, " %d", &spu->code[spu->ip]);
        fprintf(stderr, " %d\n", spu->code[spu->ip]);
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
        switch(spu->code[spu->ip])
        {
            case PUSH:
            {
                UniPush(spu);
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
                UniPop(spu);
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
                fprintf(stderr, "Programm finished with no errors.\n");
                Loop_flag = 0;
                //fclose(Prog_code);
                break;
            }

            default:
            {
                fprintf(stderr, "Sintax Error: '%d; ip = %d'\n", spu->code[spu->ip], spu->ip);
                // fclose(Prog_code);
                Loop_flag = 0;
            }

        }
    }

}



static int UniPush(SPU_t* spu)
{
    //fprintf(stderr, "## VAL FOR IP = %d\n", spu->code[spu->ip]);
    (spu->ip)++;
    //fprintf(stderr, "## VAL FOR IP + 1 = %d\n", spu->code[spu->ip]);
    uint32_t ArgCode = spu->code[spu->ip];
    uint32_t result = 0;

    if(ArgCode & REG_MASK)
    {
        //fprintf(stderr, "## IS REG\n");
        (spu->ip)++;
        result = spu->registers[spu->code[spu->ip]];
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
        uint32_t addr = result;
        result = spu->ram[addr];
    }

    StackPush(&spu->stk, result);

    (spu->ip)++;

    return 0; //TODO err codes
}


static int UniPop(SPU_t* spu)
{
    (spu->ip)++;
    uint32_t ArgCode = spu->code[spu->ip];

    int* result = 0;
    int  tres = 0;

    if(ArgCode & REG_MASK)
    {
        (spu->ip)++;
        tres = spu->code[spu->ip];
        result = &spu->registers[spu->code[spu->ip]];
    }

    if(ArgCode & RAM_MASK)
    {
        if(ArgCode & REG_MASK && ArgCode & C_MASK)
        {
            (spu->ip)++;
            tres += spu->code[spu->ip];
            result = &spu->ram[tres];
        }

        else if(ArgCode & C_MASK)
        {
            (spu->ip)++;
            tres = spu->code[spu->ip];
            result = &spu->ram[tres];
        }

        else
        {
            (spu->ip)++;
            result = &spu->ram[tres];
        }
    }

    StackPop(&spu->stk, result);

    (spu->ip)++;

    return 0; //TODO err codes
}




