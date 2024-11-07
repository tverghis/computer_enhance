#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/vector.h"

char *REG_BYTE[8] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
char *REG_WORD[8] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};

char *reg_name(uint8_t reg, uint8_t w) {
    return w == 0 ? REG_BYTE[reg] : REG_WORD[reg];
}

char *disasm_mov(uint8_t *opcode, char *buf) {
    uint8_t d = (opcode[0] & 0x02) >> 1;
    uint8_t w = opcode[0] & 0x01;
    uint8_t operands = opcode[1];

    uint8_t reg = (operands & 0x38) >> 3;
    uint8_t reg_mem = (operands & 0x07);

    switch (d) {
    case 0:
        sprintf(buf, "mov %s, %s", reg_name(reg_mem, w), reg_name(reg, w));
        break;
    case 1:
        sprintf(buf, "mov %s, %s", reg_name(reg, w), reg_name(reg_mem, w));
        break;
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
        printf("Failed to open file %s.\n", argv[1]);
        return -1;
    }

    Vector items = vec_new(sizeof(char *));
    uint8_t buffer[2];

    while (fread(buffer, 1, sizeof(buffer), f) != 0) {
        char *instr = malloc(16);
        instr = disasm_mov(buffer, instr);
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
