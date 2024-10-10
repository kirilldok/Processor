#ifndef STACK_Debug
#define STACK_Debug

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <math.h>

#include "hash.h"
#include "StackFunc.h"


#ifdef NDEBUG
#define ON_DEBUG(code)
#else
#define ON_DEBUG(code) code
#endif

typedef int Canary_t;

const size_t UNLIKELY_STACKSIZE = 999999999999999; // Very unlikely to appear, used as a sign of stack underflow.
const Canary_t LeftStructCanaryREF  = 12000;
const Canary_t RightStructCanaryREF = 12100;
const Canary_t LeftDataCanaryREF = 13000;
const Canary_t RightDataCanaryREF = 13100;


void StackAssert(Stack_t* stk, const char* stkName, const char* file, const char* func, int line);
size_t StackError(Stack_t* stk);
// int VoidIntDump(Stack_t* stk);
int StackDump_t(Stack_t* stk, const char* stkName, const char* file, const char* func, int line);



#ifndef STACK_DUMP_INC
#define StackDump(stk) StackDump_t((stk), #stk, __FILE__, __func__, __LINE__)
#endif


#ifndef NDEBUG
#define $ fprintf(stderr, "%s:%d\n", __FILE__, __LINE__)
#define STACK_ASSERT(Stack) StackAssert((Stack), #Stack, __FILE__, __func__, __LINE__)
#define STACK_INIT(Stack, inCap) strcat(*Stack.stkName, #Stack); StackInit((Stack), inCap)
#else
#define STACK_ASSERT(Stack)
#endif

#endif
