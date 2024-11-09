#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

char *REG_BYTE[8] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
char *REG_WORD[8] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};

#define REG_NAME(reg, w) ((w) == 0 ? REG_BYTE[(reg)] : REG_WORD[(reg)])

#define MASK_OPCODE 0b11111100
#define MASK_D 0b00000010
#define MASK_W 0b00000001
#define MASK_MOD 0b11000000
#define MASK_REG 0b00111000
#define MASK_RM 0b00000111

static void disasm_mov(uint8_t *instrs, char *buf) {
    uint8_t d = (instrs[0] & MASK_D) >> 1;
    uint8_t w = instrs[0] & MASK_W;
    uint8_t operands = instrs[1];

    uint8_t mod = (operands & MASK_MOD) >> 6;
    uint8_t reg = (operands & MASK_REG) >> 3;
    uint8_t reg_mem = (operands & MASK_RM);

    switch (d) {
    case 0:
        sprintf(buf, "mov %s, %s", REG_NAME(reg_mem, w), REG_NAME(reg, w));
        break;
    case 1:
        sprintf(buf, "mov %s, %s", REG_NAME(reg, w), REG_NAME(reg_mem, w));
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
