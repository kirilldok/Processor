#include "ProcessorFunc.h"
#include"../MyStack/StackFunc.h"

static const size_t default_stack_size = 4;

int SpuCtor(SPU_t* spu)
{
    assert(spu);

    spu->code = (Code_t*)calloc(default_max_code_size, sizeof(Code_t)); assert(spu->code);
    //fprintf(stderr, "size of code = %lu\n", sizeof(spu->code));
    spu->ip = 0;
    spu->code_size = 0;
    StackCtor(&spu->stk, default_stack_size); assert(&spu->stk);
    StackCtor(&spu->return_codes, default_max_code_size); assert(&spu->return_codes);

    SpuDump(spu);
    return 0;
}

size_t CodeResize(SPU_t* spu, size_t oldcapacity)
{
    assert(spu);
    static const int ReallocCoef = 2;
    size_t newcapacity = ReallocCoef * oldcapacity;

    spu->code = (Code_t*)realloc(spu->code, newcapacity * sizeof(Code_t)); assert(spu->code);

    return newcapacity;
}

int SpuDtor(SPU_t* spu)
{
    assert(spu);

    StackDtor(&spu->stk);
    StackDtor(&spu->return_codes);
    free(spu->code); //spu->code = NULL;

    return 0;
}

int SPUDump(SPU_t* spu, const char* file, const char* func, int line)
{
    //fprintf(stderr, "Dump opened\n");
    FILE* Dump = fopen("run/SPU_Dump.txt", "a+");
    assert(Dump);

    fprintf(Dump,
        "########################## SPU INFO ##########################\n"
        "## CALLER SPU: %p\n"
        "## CALLER FILE    : %s\n"
        "## CALLER FUNCTION: %s\n"
        "## LINE           = %d\n"
        "## SIZE OF CODE   = %lu\n"
        "## SPU CODE PTR: %p\n",
        spu, file, func, line, spu->code_size, spu->code);

    fprintf(Dump, "## CODE:\n\t");
    {
        for(size_t i = 0; i < spu->code_size; i++)
            fprintf(Dump, "%.2lu ", i);
        fprintf(Dump, "\n\t");

        for(size_t i = 0; i < spu->code_size; i++)
            fprintf(Dump, "%.2x ", (int)(spu->code[i]));
        fprintf(Dump, "\n\t");

        for(size_t i = 0; i < spu->ip; i++)
            fprintf(Dump, "___");
        fprintf(Dump, "^ip = %lu\n", spu->ip);
    }

    fprintf(Dump, "## REGISTERS:\n\t");
    fprintf(Dump, "ZX = %lg;  AX = %lg;  BX = %lg;  CX = %lg; DX = %lg;  MLR = %lg;\n",
                   spu->registers[ZX], spu->registers[AX], spu->registers[BX],
                   spu->registers[CX], spu->registers[DX], spu->registers[MLR]);
    //fprintf(stderr, "registers written\n");

    if ((spu->stk.capacity) != 0)
    {
        fprintf(Dump, "## STACK = %p\n\t", &spu->stk);
        {
            for(size_t i = 0; i < ((spu->stk.size) ); i++)
            {
                fprintf(Dump, "%lg ", spu->stk.data[i]);
            }

            fprintf(Dump, "\n");
        }
        Stack_Dump(&spu->stk);
    }
    //fprintf(stderr, "spu dumped into file\n");

    fprintf(Dump, "\n################################################################\n\n\n\n");
    fclose(Dump);

    return 0;
}
