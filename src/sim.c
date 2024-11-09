#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/disasm.h"
#include "../include/vector.h"

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
