#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/vector.h"

#define VEC_INIT_CAP 8

Vector vec_new(size_t item_size) {
    Vector self = {.cap = 0, .len = 0, .item_size = item_size, .head = NULL};

    return self;
}

static int vec_grow(Vector *v) {
    if (v->cap == 0) {
        void *addr = calloc(VEC_INIT_CAP, v->item_size);
        if (addr == NULL) {
            return -1;
        }
        v->head = addr;
        v->cap = VEC_INIT_CAP;
        return 0;
    }

    void *addr = calloc(v->cap * 2, v->item_size);
    memcpy(addr, v->head, v->len * v->item_size);
    void *old_head = v->head;
    v->cap *= 2;
    v->head = addr;
    free(old_head);

    return 0;
}

int vec_append(Vector *v, void *item) {
    if (v->len == v->cap) {
        if (vec_grow(v) != 0) {
            return -1;
        }
    }

    memcpy((char *)v->head + (v->item_size * v->len), item, v->item_size);
    v->len++;

    return 0;
}

void *vec_index(Vector *v, size_t idx) {
    if (idx > v->len) {
        return NULL;
    }

    return ((char *)v->head + (idx * v->item_size));
}

void vec_free(Vector *v) { free(v->head); }
