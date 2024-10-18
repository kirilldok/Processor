#include "Processor.h"
#include"StackFunc.h"



int SPUDump_t(SPU_t* spu, const char* file, const char* func, int line)
{
    fprintf(stderr, "Dump opened\n");
    FILE* Dump = fopen("SPU_Dump.txt", "a+");
    assert(Dump);


    fprintf(Dump,
        "########################## SPU INFO ##########################\n"
        "## CALLER SPU: %p\n"
        "## CALLER FILE    : %s\n"
        "## CALLER FUNCTION: %s\n"
        "## LINE           = %d\n"
        "## SIZE OF CODE   = %zu\n"
        "## SPU CODE PTR: %p\n",
        spu, file, func, line, spu->code_size, spu->code);



    fprintf(Dump, "## CODE:\n\t");
    {
        for(size_t i = 0; i < spu->code_size; i++)
        {
            fprintf(Dump, "%.2lu ", i);
        }

        fprintf(Dump, "\n\t");

        for(size_t i = 0; i <spu->code_size; i++)
        {
            fprintf(Dump, "%.2x ", (unsigned)(spu->code[i]));
        }

        fprintf(Dump, "\n\t");

        for(size_t i = 0; i < spu->ip; i++)
            fprintf(Dump, "___");
        fprintf(Dump, "^ip = %lu\n", spu->ip);
    }

    fprintf(Dump, "## REGISTERS:\n\t");
    fprintf(Dump, "ZX = %d;  AX = %d;  BX = %d;  CX = %d; DX = %d;  MLR = %d;\n",
                   spu->registers[ZX], spu->registers[AX], spu->registers[BX],
                   spu->registers[CX], spu->registers[DX], spu->registers[MLR]);
    fprintf(stderr, "registers written\n");

    if ((spu->stk.capacity) != 0)
    {
        fprintf(Dump, "## STACK = %p\n\t", &spu->stk);
        {
            for(size_t i = 1; i < ((spu->stk.size) ); i++)
            {
                fprintf(Dump, "%d ", spu->stk.data[i]);
            }

            for(size_t i = 1; i < ((spu->stk.capacity) ); i++)
            {
                fprintf(Dump, "%d ", spu->stk.data[i]);
            }

            fprintf(Dump, "\n");
        }
        Stack_Dump(&spu->stk);
    }


    fprintf(stderr, "spu dumped into file\n");



    fprintf(Dump, "\n################################################################\n\n\n\n");
    fclose(Dump);

    return 0;
}
