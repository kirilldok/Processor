#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdint.h>

#include "Stackdebug.h"



int main()
{
    #ifndef NDEBUG
    FILE* clean = fopen("log.txt", "w+b");
    fclose(clean);
    #endif

    Stack_t stk = { 0 };
    StackCtor(&stk, 32);

    StackPush(&stk, 100);
    StackDump(&stk);

    StackPush(&stk, 200);
    StackDump(&stk);

    StackElem_t  x = 0;


    StackPush(&stk, 300);
    StackDump(&stk);

    StackPop(&stk, &x);
    printf("x = %d, stk.size = %lu\n", x, stk.size);
    StackDump(&stk);

    StackDtor(&stk);
    return 0;


}
