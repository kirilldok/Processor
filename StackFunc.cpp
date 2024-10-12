#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdint.h>
#include "StackFunc.h"
#include "Stackdebug.h"



int StackCtor(Stack_t* stk, size_t stacklen)
{
    if(stacklen == NULL)
    {
        printf("Length of stack must be a natural number!\n");
        return STACK_UNDERFLOW;
    }
    stk->capacity  = stacklen;

    stk->data = (StackElem_t*)calloc(stk->capacity ON_DEBUG(+ 2), sizeof(StackElem_t));

    if(stk->data == NULL)
    {
        fprintf(stderr, "Dynamic Memory dead\n");
        stk->Error += ALLOC_ERROR;
        return ALLOC_ERROR;
    }

    stk->size = 0;



    #ifndef NDEBUG
        stk->RStructCanary = RightStructCanaryREF;
        stk->LStructCanary = LeftStructCanaryREF;
        *(stk->data) = LeftDataCanaryREF;
        *(StackElem_t*)((char*)stk->data + (stk->capacity ) * sizeof(StackElem_t) + sizeof(Canary_t)  ) = RightDataCanaryREF;
    #endif //NDEBUG


    for (size_t i = 0; i < stk->capacity; i++)
    {
        *(StackElem_t*)((char*)stk->data + i * sizeof(StackElem_t) ON_DEBUG(+ sizeof(Canary_t))) = Poison;
    }

    #ifndef NDEBUG
        stk->HashSum = hash(stk->data, stk->capacity + 2);
    #endif
    StackDump(stk);
    return NO_ERRORS;
}


int StackResize(Stack_t* stk, bool resizeflag)
{

    STACK_ASSERT(stk);

    if (resizeflag)
    {
        STACK_ASSERT(stk);

        stk->data = (StackElem_t*)realloc(stk->data, ReallocCoef * stk->capacity * sizeof(StackElem_t) ON_DEBUG( + 2 * sizeof(Canary_t)));
        stk->capacity = ReallocCoef * stk->capacity;


        #ifndef NDEBUG
            stk->HashSum = hash(stk->data, stk->capacity + 2);
            *(StackElem_t*)((char*)stk->data + (stk->capacity + 1)*sizeof(StackElem_t)) = RightDataCanaryREF;
        #endif

        STACK_ASSERT(stk);
        return NO_ERRORS;
    }
    else
    {

        STACK_ASSERT(stk);

        size_t new_capacity = stk->capacity / ReallocCoef;

        stk->data = (StackElem_t*)realloc( stk->data, new_capacity * sizeof(StackElem_t) ON_DEBUG( + 2 * sizeof(Canary_t)));
        stk->capacity = new_capacity;

        #ifndef NDEBUG
            *(StackElem_t*)((char*)stk->data + (stk->capacity) * sizeof(StackElem_t) + sizeof(Canary_t)) = RightDataCanaryREF;
            stk->HashSum = hash(stk->data, stk->capacity + 2);
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

    *(StackElem_t*)((char*)stk->data + stk->size * sizeof(StackElem_t) ON_DEBUG(+ sizeof(Canary_t))) = element;
    ++stk->size;

    for (size_t i = stk->size; i < stk->capacity; i++)
    {
        *(StackElem_t*)((char*)stk->data + i * sizeof(StackElem_t) ON_DEBUG(+ sizeof(Canary_t))) = Poison;
    }

    #ifndef NDEBUG
            *(StackElem_t*)((char*)stk->data + (stk->capacity)*sizeof(StackElem_t) + sizeof(Canary_t)) = RightDataCanaryREF;
            stk->HashSum = hash(stk->data, stk->capacity + 2);
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
    *Pop_element = *(StackElem_t*)((char*)stk->data + stk->size * sizeof(StackElem_t) ON_DEBUG(+ sizeof(Canary_t)));
    *(StackElem_t*)((char*)stk->data + stk->size * sizeof(StackElem_t) ON_DEBUG(+ sizeof(Canary_t))) = Poison;


    #ifndef NDEBUG
        *(StackElem_t*)((char*)stk->data + (stk->capacity)*sizeof(StackElem_t) + sizeof(Canary_t)) = RightDataCanaryREF;
        stk->HashSum = hash(stk->data, stk->capacity + 2);
    #endif

    STACK_ASSERT(stk);

    return NO_ERRORS;
}



int StackDtor(Stack_t* stk)
{

    STACK_ASSERT(stk);

    for(size_t i = 0; i < stk->size; i++)
    {
        stk->data[i] = NULL;
    }

    stk->size = NULL;
    stk->capacity = NULL;
    stk->LStructCanary = NULL;
    stk->RStructCanary = NULL;
    stk->HashSum = NULL;

    free(stk->data);stk->data = NULL;

    return NO_ERRORS;
}



int Stack_Dump(Stack_t* stk)
{
    StackDump(stk);
    return NO_ERRORS;
}
