#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>

#include "Stackdebug.h"
#include "StackFunc.h"
#include "hash.h"

void StackAssert(Stack_t* stk, const char* file, const char* func, int line)
{
    //printf("Stack assert opened\n");

    if(stk == NULL)
    {

        fprintf(stderr, "Stack pointer is NULL");
        assert(0);
    }


    if((stk->Error = StackError(stk)) != 0)
    {
       fprintf(stderr, "CALLER STACK: %p\n"
                       "CALLER FILE    : %s\n"
                       "CALLER FUNCTION: %s\n"
                       "LINE = %d\n", stk, file, func, line);
        StackDump(stk);
        fprintf(stderr, "Emergency exit, stack dumped into log.txt\n");
        assert(0);
    }
    //fprintf(stderr,"stack assert closed\n");
}




int StackError(Stack_t* stk)
{

    int errFlag = NO_ERRORS;
    if(stk == NULL)
    {
        //fprintf(stderr, "Stack pointer is NULL\n");
        errFlag |= STACK_PTR_IS_NULL;
    }

    if(stk->data == NULL)
    {
        //fprintf(stderr, "Data pointer is NULL\n");
        errFlag |= DATA_PTR_IS_NULL;
    }

    if(stk->size > stk->capacity)
    {
        //fprintf(stderr, "Stack overflow\n");
        errFlag |= STACK_OVERFLOW;
    }

    if(stk->size > UNLIKELY_STACKSIZE)
    {
        //fprintf(stderr, "Stack has negative size (stack underflow)\n");
        errFlag |= STACK_UNDERFLOW;
    }

    if((*(Canary_t*)((char*)stk->data - sizeof(Canary_t))) != LeftDataCanaryREF)
    {
        //fprintf(stderr, "left data canary is damaged\n");
        errFlag |= LEFT_DATA_CANARY_CORRUPTED;
    }

    if(*(Canary_t*)(stk->data + stk->capacity) != RightDataCanaryREF)
    {
        //fprintf(stderr, "right data canary is damaged\n");
        errFlag |= RIGHT_DATA_CANARY_CORRUPTED;
    }

    if(stk->LStructCanary != LeftStructCanaryREF)
    {
        //fprintf(stderr, "left struck canary is damaged\n");
        errFlag |= LEFT_STACK_CANARY_CORRUPTED;
    }

    if(stk->RStructCanary != RightStructCanaryREF)
    {
        //fprintf(stderr, "right struck canary is damaged\n");
        errFlag |= RIGHT_STACK_CANARY_CORRUPTED;
    }

    if(stk->HashSum != hash(stk->data, stk->capacity))
    {

        //fprintf(stderr, "data is dameged.   \n");
        errFlag |= DATA_CORRUPTED;
    }

    return errFlag;
}






int StackDump_t(Stack_t* stk, const char* file, const char* func, int line)
{
    //printf("Dump raw opened\n");


    FILE* log = fopen("log.txt", "a+b");
    if(log == NULL)
    {
        stk->Error = FILE_CREATION_ERROR;
        fclose(log);
        return FILE_CREATION_ERROR;
    }

    fprintf(log,
    "########################## STACK INFO ##########################\n"
    "## CALLER STACK: %p\n"
    "## CALLER FILE    : %s\n"
    "## CALLER FUNCTION: %s\n"
    "## LINE          = %d\n"
    "## STACK SIZE    = %zu\n"
    "## STACK CAPACIY = %zu\n"
    "## LEFT STACK CANARY = %d\n"
    "## RIGHT STACK CANARY = %d\n"
    "## STACK DATA PTR: %p\n",
    stk, file, func, line, stk->size, stk->capacity, stk->LStructCanary, stk->RStructCanary,  stk->data);


    fprintf(log, "## DATA BUFFER: \n");

    if (((stk->Error >> 8) & 1) != 0)
        fprintf(log, "### LEFT CANARY: %d\n", *(Canary_t*)((char*)stk->data - sizeof(Canary_t)));


    if((stk->capacity < 64) && (stk->size < 64))
    {
        for(size_t i = 0; i < stk->capacity; i++)
            fprintf(log, "# [%d]  (%zu)\n", stk->data[i], i);
    }
    else
    {
        for(size_t i = 0; i < stk->size; i++)
            fprintf(log, "# [%d]  (%zu)\n", stk->data[i], i);
    }



    if ((( stk->Error >> 9) & 1) != 0)
        fprintf(log, "### RIGHT CANARY: %d\n", *(Canary_t*)(stk->data + stk->capacity));


    fprintf(log, "## HASH SUM: %zu\n", stk->HashSum);
    fprintf(log, "## ERRORS: ");
    fprintf(log, "%d\n", stk->Error);


    fprintf(log, "\n################################################################\n\n\n\n");
    fclose(log);

    //fprintf(stderr,"Data logged in Dump\n");

    return NO_ERRORS;
}

