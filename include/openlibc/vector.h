#ifndef OPENLIBC_VECTOR_H
#define OPENLIBC_VECTOR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct vector {
    void **data;

    size_t length;
    size_t capacity;
} vector_t;

extern const vector_t EMPTY_VECTOR;

void vector_init(vector_t *vector, size_t capacity);

void vector_destroy(vector_t *vector);

int vector_index_of(vector_t *vector, const void *element);

void vector_add(vector_t *vector, void *element);

void *vector_pop(vector_t *vector);

void vector_insert_at(vector_t *vector, size_t index, void *element);

void vector_remove(vector_t *vector, void *element);

void *vector_remove_at(vector_t *vector, size_t index);


#ifdef __cplusplus
};
#endif

#endif  /* OPENLIBC_VECTOR_H */