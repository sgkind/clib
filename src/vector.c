#include "openlibc/vector.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

const vector_t EMPTY_VECTOR = {NULL, 0, 0};

void vector_init(vector_t *vector, size_t capacity)
{
    vector->length = 0;
    vector->capacity = capacity;
    if (capacity > 0) {
        vector->data = malloc(sizeof(void*) * capacity);
    } else {
        vector->data = NULL;
    }
}

void vector_destroy(vector_t *vector)
{
    if (vector->capacity > 0) {
        free(vector->data);
    }
}

static void enlarge_vector_if_full(vector_t *vector)
{
    if (vector->length >= vector->capacity) {
        if (vector->capacity) {
            size_t old_bytes = sizeof(void *) * vector->capacity;
            vector->capacity *= 2;
            size_t bytes = sizeof(void *) * vector->capacity;
            void **temp = malloc(bytes);
            memcpy(temp, vector->data, old_bytes);
            free(vector->data);
            vector->data = temp;
        } else {
            // null vector, need not free data
            vector->capacity = 2;
            vector->data = malloc(sizeof(void *) * vector->capacity);
        }
    }
}

void vector_add(vector_t *vector, void *element)
{
    enlarge_vector_if_full(vector);
    assert(vector->data);
    assert(vector->length < vector->capacity);
    vector->data[vector->length++] = element;
}

void *vector_pop(vector_t *vector)
{
    if (vector->length == 0) {
        return NULL;
    }

    return vector->data[--vector->length];
}

int vector_index_of(vector_t *vector, const void *element)
{
    for (size_t i = 0; i < vector->length; ++i) {
        if (vector->data[i] == element) {
            return i;
        }
    }

    return -1;
}

void vector_insert_at(vector_t *vector, size_t index, void *element)
{
    assert(index >= 0);
    assert(index <= vector->length);
    enlarge_vector_if_full(vector);
    ++vector->length;
    memmove(&vector->data[index+1], &vector->data[index],
            sizeof(void*) * (vector->length - index - 1));
    vector->data[index] = element;
}

void vector_remove(vector_t *vector, void *element)
{
    int index = vector_index_of(vector, element);
    if (index == -1) {
        return;
    }
    vector_remove_at(vector, index);
}

void *vector_remove_at(vector_t *vector, size_t index)
{
    assert(index >= 0);
    assert(index < vector->length);
    void *element = vector->data[index];
    memmove(&vector->data[index], &vector->data[index+1],
            sizeof(void*) * (vector->length - index - 1));
    --vector->length;
    return element;
}