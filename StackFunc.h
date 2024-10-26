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



typedef double StackElem_t;

// static const StackElem_t Poison = -666.6;


enum errors
{
    UNDEFINED_ERROR              = 1 << 13,
    ALLOC_ERROR                  = 1 << 0,
    DESTRUCTOR_ERROR             = 1 << 1,
    FILE_CREATION_ERROR          = 1 << 2,
    INPUT_ERROR                  = 1 << 3,
    STACK_UNDERFLOW              = 1 << 4,
    STACK_PTR_IS_NULL            = 1 << 5,
    DATA_PTR_IS_NULL             = 1 << 6,
    STACK_OVERFLOW               = 1 << 7,
    LEFT_DATA_CANARY_CORRUPTED   = 1 << 8,
    RIGHT_DATA_CANARY_CORRUPTED  = 1 << 9,
    LEFT_STACK_CANARY_CORRUPTED  = 1 << 10,
    RIGHT_STACK_CANARY_CORRUPTED = 1 << 11,
    DATA_CORRUPTED               = 1 << 12,
    NO_ERRORS                    = 0,
};



static const int STACK_NAME_MAX = 16;
static const int FUNC_NAME_MAX= 32;

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


    int Error;

    #ifndef NDEBUG
    int RStructCanary;
    #endif
} Stack_t;


int StackCtor(Stack_t* stk, size_t stacklen);

int StackPush(Stack_t* stk, StackElem_t element);
int StackPop(Stack_t* stk, StackElem_t* POPelement);
int StackResize(Stack_t* stk, size_t new_capacity);

int StackDtor(Stack_t* stk);
int Stack_Dump(Stack_t* stk);


#endif
