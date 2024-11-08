#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/vector.h"

char *REG_BYTE[8] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
char *REG_WORD[8] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};

#define REG_NAME(reg, w) ((w) == 0 ? REG_BYTE[(reg)] : REG_WORD[(reg)])

void disasm_mov(uint8_t *instrs, char *buf) {
    uint8_t d = (instrs[0] & 0b00000010) >> 1;
    uint8_t w = instrs[0] & 0b00000001;
    uint8_t operands = instrs[1];

    uint8_t reg = (operands & 0b00111000) >> 3;
    uint8_t reg_mem = (operands & 0b00000111);

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

    uint8_t opcode = (instrs[0] & 0b11111100) >> 2;

    switch (opcode) {
    case 0b100010:
        disasm_mov(instrs, buf);
        break;
    default:
        fprintf(stderr, "disasm: unrecognized opcode %b\n", opcode);
    }

    return buf;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("USAGE: sim <file>\n");
        return -1;
    }

    FILE *f = fopen(argv[1], "rb");

    if (f == NULL) {
        fprintf(stderr, "Failed to open file %s.\n", argv[1]);
        return -1;
    }

    Vector items = vec_new(sizeof(char *));
    uint8_t buffer[2];

    while (fread(buffer, 1, sizeof(buffer), f) != 0) {
        char *instr = disasm(buffer);

        if (instr == NULL) {
            return -1;
        }

        vec_append(&items, &instr);
    }

    for (size_t i = 0; i < items.len; ++i) {
        char *item = *(char **)vec_index(&items, i);
        printf("%s\n", item);
        free(item);
    }

    vec_free(&items);
    fclose(f);

    return 0;
}
