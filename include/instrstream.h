#ifndef _INSTRSTREAM_H
#define _INSTRSTREAM_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    void *inner;
} InstrStream;

size_t next_byte(InstrStream *, uint8_t *);

#endif
