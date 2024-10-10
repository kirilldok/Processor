#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>

#include "hash.h"


#ifndef NDEBUG
#define ON_DEBUG(code) code
#endif



typedef int StackElem_t;
const StackElem_t Poison = -6666;
// const StackElem_t Poison = -666.6;


enum errors
{
    UNDEFINED_ERROR = 13,
    ALLOC_ERROR = 1,
    DESTRUCTOR_ERROR = 10,
    FILE_CREATION_ERROR = 100,
    INPUT_ERROR = 1000,
    STACK_UNDERFLOW = 10000,
    STACK_PTR_IS_NULL = 100000,
    DATA_PTR_IS_NULL = 1000000,
    STACK_OVERFLOW = 1000000,
    LEFT_DATA_CANARY_CORRUPTED = 10000000,
    RIGHT_DATA_CANARY_CORRUPTED = 100000000,
    LEFT_STACK_CANARY_CORRUPTED = 1000000000,
    RIGHT_STACK_CANARY_CORRUPTED = 10000000000,
    DATA_CORRUPTED = 333,
    NO_ERRORS = 0,
};


const int STACK_NAME_MAX = 16;
const int FUNC_NAME_MAX= 32;

typedef struct Stack_t
{
    #ifndef NDEBUG
    int LStructCanary;

    hash_t HashSum;

    char stkName[STACK_NAME_MAX];
    char file[FILENAME_MAX];
    const char func[FUNC_NAME_MAX];
    const int line;
    #endif

    StackElem_t* data;
    size_t size;
    size_t capacity;


    size_t Error;

    #ifndef NDEBUG
    int RStructCanary;
    #endif
} Stack_t;

const int ReallocCoef = 2;


int StackCtor(Stack_t* stk, size_t stacklen);

int StackPush(Stack_t* stk, StackElem_t element);
int StackPop(Stack_t* stk, StackElem_t* POPelement);
int StackResize(Stack_t* stk, bool resizeflag);

int StackDtor(Stack_t* stk);

int Stack_Dump(Stack_t* stk);
//int Poising(StackElem_t* data[], size_t lenofNOTPoising, size_t ArrSize);

#endif
