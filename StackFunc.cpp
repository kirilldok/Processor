#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdint.h>
#include "StackFunc.h"
#include "Stackdebug.h"



int StackCtor(Stack_t* stk, size_t stacklen)
{
    if(stacklen == 0)
    {
        printf("Length of stack must be a natural number!\n");
        return STACK_UNDERFLOW;
    }
    stk->capacity  = stacklen;

    char* data_with_canaries = (char*)calloc(stk->capacity ON_DEBUG(+ 2), sizeof(StackElem_t));
    stk->data = (StackElem_t*)(data_with_canaries + sizeof(Canary_t));

    if(data_with_canaries == NULL)
    {
        fprintf(stderr, "Dynamic Memory dead\n");
        stk->Error += ALLOC_ERROR;
        return ALLOC_ERROR;
    }

    stk->size = 0;

    #ifndef NDEBUG
        stk->RStructCanary = RightStructCanaryREF;
        stk->LStructCanary = LeftStructCanaryREF;
        *(Canary_t*)(data_with_canaries) = LeftDataCanaryREF;
        *(Canary_t*)(stk->data + stk->capacity) = RightDataCanaryREF;
        for (size_t i = 0; i < stk->capacity; i++)
        {
            stk->data[i] = Poison;
        }
        stk->HashSum = hash(stk->data, stk->capacity);
    #endif

    StackDump(stk);
    return NO_ERRORS;
}


int StackResize(Stack_t* stk, bool Decrease)
{

    STACK_ASSERT(stk);

    if (Decrease)
    {
        STACK_ASSERT(stk);

        char* data_with_canaries = (char*)realloc((char*)stk->data - sizeof(Canary_t),
                                                        ReallocCoef *
                                                        stk->capacity * sizeof(StackElem_t)
                                                        ON_DEBUG( + 2 * sizeof(Canary_t)));
        stk->data = (StackElem_t*)(data_with_canaries + sizeof(Canary_t));

        stk->capacity = ReallocCoef * stk->capacity;



        #ifndef NDEBUG
            for (size_t i = stk->size; i < stk->capacity; i++)
            {
                stk->data[i] = Poison;
            }
            *(Canary_t*)(data_with_canaries) = LeftDataCanaryREF;
            *(Canary_t*)(stk->data + stk->capacity) = RightDataCanaryREF;
            stk->HashSum = hash(stk->data, stk->capacity);
        #endif

        STACK_ASSERT(stk);
        return NO_ERRORS;
    }
    else
    {

        STACK_ASSERT(stk);

        size_t new_capacity = stk->capacity / ReallocCoef;

        char* data_with_canaries = (char*)realloc((char*)stk->data - sizeof(Canary_t),
                                                            new_capacity *
                                                            sizeof(StackElem_t)
                                                            ON_DEBUG(+ 2 * sizeof(Canary_t)));
        stk->data = (StackElem_t*)(data_with_canaries + sizeof(Canary_t));

        stk->capacity = new_capacity;


        #ifndef NDEBUG
            *(Canary_t*)(data_with_canaries) = LeftDataCanaryREF;
            *(Canary_t*)(stk->data + stk->capacity) = RightDataCanaryREF;
            stk->HashSum = hash(stk->data, stk->capacity);
        #endif

        STACK_ASSERT(stk);

        return NO_ERRORS;
    }
    STACK_ASSERT((stk));
    return ALLOC_ERROR;
}


int StackPush(Stack_t* stk, StackElem_t element)
{

    STACK_ASSERT(stk);

    if (stk->size  >=  stk->capacity)
    {
        StackResize(stk, true);
    }

    stk->data[stk->size] = element;
    ++stk->size;
    StackDump(stk);


    #ifndef NDEBUG
        for (size_t i = stk->size; i < stk->capacity; i++)
        {
            stk->data[i] = Poison;
        }

        stk->HashSum = hash(stk->data, stk->capacity);
    #endif

    STACK_ASSERT(stk);
    return NO_ERRORS;
}


int StackPop(Stack_t* stk, StackElem_t* Pop_element)
{

    if(Pop_element == NULL)
    {
        printf("Elemment of stack must be StakElem_t type!\n");
        return INPUT_ERROR;
    }

    STACK_ASSERT(stk);

    if (stk->size <= (stk->capacity / 4))
    {
        if (StackResize(stk, false) != 0)
        {
            stk->Error += ALLOC_ERROR;
            free(stk->data);
            return ALLOC_ERROR;
        }
    }


    stk->size = stk->size - 1;
    *Pop_element = stk->data[stk->size];



    #ifndef NDEBUG
        stk->data[stk->size] = Poison;
        stk->HashSum = hash(stk->data, stk->capacity);
    #endif

    STACK_ASSERT(stk);

    return NO_ERRORS;
}



int StackDtor(Stack_t* stk)
{

    STACK_ASSERT(stk);

    for(size_t i = 0; i < stk->size; i++)
    {
        stk->data[i] = 0;

    }

    stk->size = 0;
    stk->capacity = 0;

    #ifndef NDEBUG
        stk->LStructCanary = 0;
        stk->RStructCanary = 0;
        stk->HashSum = 0;

       free((char*)stk->data - sizeof(Canary_t));
       //free(stk->data);stk->data = NULL;
    #else
        free(stk->data);stk->data = NULL;
    #endif

    fprintf(stderr, "Stack killed\n");

    return NO_ERRORS;
}



int Stack_Dump(Stack_t* stk)
{
    StackDump(stk);
    return NO_ERRORS;
}
