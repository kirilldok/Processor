#ifndef COMMANDS_INCLUDED
#define COMMANDS_INLUDED

#include <stdlib.h>
#include <inttypes.h>

enum Machine_Commands
{
    PUSH = 0x0001,
    PUSHR = 0x0002,
    ADD = 0x0003,
    SUB = 0x0004,
    DIV = 0x0005,
    MUL = 0x0006,
    POW = 0x0007,
    SQRT = 0x0008,
    SIN = 0x0009,

    POP = 0x000B,
    OUT = 0x0010,
    DUMP = 0x0011,
    JMP = 0x0014,
    JB = 0x0015,
    JA = 0x0016,
    JE = 0x0017,
    JNE = 0x0018,
    JBE = 0x0019,
    JAE = 0x001A,

    HLT = 0x007F,
};

const uint32_t RAM_REG_CONSTVAL = 7;
const uint32_t RAM_REG = 6;
const uint32_t RAM_CONSTVAL = 5;
const uint32_t REG_CONSTVAL = 3;
const uint32_t REG = 2;
const uint32_t CONSTVAL = 1;

typedef struct Header
{
    uint32_t signature;
    uint32_t version;
    uint32_t size;
    uint32_t reserved;
} Header;

static const int size_of_header = 4;


enum registers
{
    ZX  = 0,
    AX  = 1,
    BX  = 2,
    CX  = 3,
    DX  = 4,
    MLR = 5,
};

#endif
