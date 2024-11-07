#ifndef _VECTOR_H
#define _VECTOR_H

#include <stddef.h>

typedef struct {
    size_t cap;
    size_t len;
    size_t item_size;
    void *head;
} Vector;

Vector vec_new(size_t);
int vec_append(Vector *, void *);
void *vec_index(Vector *, size_t);
void vec_free(Vector *);

#endif
