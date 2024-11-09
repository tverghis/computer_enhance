#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/disasm.h"

const char *REG_BYTE[8] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
const char *REG_WORD[8] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};

InstrMov new_instrmov(uint8_t *instrs) {
    uint8_t d = (instrs[0] & MASK_D) >> 1;
    uint8_t w = instrs[0] & MASK_W;
    uint8_t operands = instrs[1];

    uint8_t mod = (operands & MASK_MOD) >> 6;
    uint8_t reg = (operands & MASK_REG) >> 3;
    uint8_t reg_mem = (operands & MASK_RM);

    InstrMov self = {.d = d,
                     .w = w,
                     .mod = mod,
                     .reg = reg,
                     .rm = reg_mem,
                     .instrs = instrs};

    return self;
}

static void disasm_mov(uint8_t *instrs, char *buf) {
    InstrMov mov = new_instrmov(instrs);

    switch (mov.d) {
    case 0:
        sprintf(buf, "mov %s, %s", REG_NAME(mov.rm, mov.w),
                REG_NAME(mov.reg, mov.w));
        break;
    case 1:
        sprintf(buf, "mov %s, %s", REG_NAME(mov.reg, mov.w),
                REG_NAME(mov.rm, mov.w));
        break;
    };
}

char *disasm(uint8_t *instrs) {
    char *buf = malloc(16);
    if (buf == NULL) {
        fprintf(stderr, "disasm: malloc failed\n");
        return NULL;
    }

    uint8_t opcode = (instrs[0] & MASK_OPCODE) >> 2;

    switch (opcode) {
    case 0b100010:
        disasm_mov(instrs, buf);
        break;
    default:
        fprintf(stderr, "disasm: unrecognized opcode %b\n", opcode);
    }

    return buf;
}
