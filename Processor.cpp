#include "Processor.h"



int SPUDump_t(SPU_t* spu,  const char* file, const char* func, int line)
{
    fprintf(stderr, "Dump opened\n");
    FILE* Dump = fopen("SPU_Dump.txt", "a+");
    assert(Dump);

    fprintf(Dump, "akhsldialhfilahdki\n");

    fprintf(Dump,
        "########################## SPU INFO ##########################\n"
        "## CALLER SPU: %p\n"
        "## CALLER FILE    : %s\n"
        "## CALLER FUNCTION: %s\n"
        "## LINE           = %d\n"
        "## SIZE OF CODE   = %zu\n"
        "## SPU CODE PTR: %p\n",
        spu, file, func, line, spu->code_size, spu->code);

    fprintf(Dump, "## CODE:\n");

    for(size_t i = 0; i <spu->code_size; i++)
    {
        fprintf(Dump, "%d ", i);
    }

    fprintf(stderr, "spu dumped into file\n");

    fclose(Dump);

    return 0;
}
