#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "../include/instrstream.h"

size_t next_byte(InstrStream *stream, uint8_t *byte) {
    return fread(byte, 1, 1, stream->inner);
}
