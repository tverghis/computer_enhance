#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "../include/instrstream.h"

InstrStream new_instrstream(void *stream) {
    InstrStream self = {.inner = stream};
    return self;
}

size_t next_byte(InstrStream *stream, uint8_t *byte) {
    return fread(byte, 1, 1, stream->inner);
}
