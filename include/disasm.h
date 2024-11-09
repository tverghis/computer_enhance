#ifndef _DISASM_H
#define _DISASM_H

#include <stdint.h>

#define REG_NAME(reg, w) ((w) == 0 ? (REG_BYTE)[(reg)] : (REG_WORD)[(reg)])

#define MASK_OPCODE 0b11111100
#define MASK_D 0b00000010
#define MASK_W 0b00000001
#define MASK_MOD 0b11000000
#define MASK_REG 0b00111000
#define MASK_RM 0b00000111

typedef struct {
    uint8_t d;
    uint8_t w;
    uint8_t mod;
    uint8_t reg;
    uint8_t rm;

    uint8_t *instrs;
} InstrMov;

InstrMov new_instrmov(uint8_t *);

char *disasm(uint8_t *);

#endif
