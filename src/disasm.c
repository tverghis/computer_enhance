#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/disasm.h"

char *REG_BYTE[8] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
char *REG_WORD[8] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};

#define REG_NAME(reg, w) ((w) == 0 ? (REG_BYTE)[(reg)] : (REG_WORD)[(reg)])

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

static void disasm_mov_reg_mode(InstrMov *mov, char **dst, char **src) {
    switch (mov->d) {
    case 0:
        *dst = REG_NAME(mov->rm, mov->w);
        *src = REG_NAME(mov->reg, mov->w);
        break;
    default:
        *dst = REG_NAME(mov->reg, mov->w);
        *src = REG_NAME(mov->rm, mov->w);
        break;
    };
}

char *EAC_BASE[] = {"bx + si", "bx + di", "bp + si", "bp + di",
                    "si",      "di",      "bp",      "bx"};
char LPAREN = '[';
char RPAREN = ']';
static void disasm_mov_mem_mode(InstrMov *mov, char **dst, char **src) {
    char *reg_name = REG_NAME(mov->reg, mov->w);
    char *mem_name = malloc(16);

    if (mem_name == NULL) {
        fprintf(stderr, "disasm_mov_mem_mode: malloc failed");
        return;
    }

    strncat(mem_name, &LPAREN, 1);
    strncat(mem_name, EAC_BASE[mov->rm], 7);
    strncat(mem_name, &RPAREN, 1);

    switch (mov->d) {
    case 0:
        *dst = mem_name;
        *src = reg_name;
        break;
    default:
        *dst = reg_name;
        *src = mem_name;
        break;
    };
}

static void disasm_mov(uint8_t *instrs, char *buf) {
    InstrMov mov = new_instrmov(instrs);

    char *dst_name = NULL;
    char *src_name = NULL;

    switch (mov.mod) {
    case 0b11:
        disasm_mov_reg_mode(&mov, &dst_name, &src_name);
        break;
    case 0b00:
        disasm_mov_mem_mode(&mov, &dst_name, &src_name);
        break;
    default:
        fprintf(stderr, "disasm_mov: unrecognized mod: %b\n", mov.mod);
        return;
    }

    sprintf(buf, "mov %s, %s", dst_name, src_name);
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
