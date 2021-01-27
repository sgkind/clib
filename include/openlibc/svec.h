/*
 * Copyright (c) 2008, 2009, 2011 Nicira, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OPENLIBC_SVEC_H
#define OPENLIBC_SVEC_H 1

#include <stdbool.h>
#include <stddef.h>

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct svec {
    char **names;
    size_t n;
    size_t allocated;
} svec_t;

#define SVEC_EMPTY_INITIALIZER { NULL, 0, 0 }

void svec_init(svec_t *);
void svec_clone(svec_t *, const svec_t *);
void svec_destroy(svec_t *);
void svec_clear(svec_t *);
bool svec_is_empty(const svec_t *);
void svec_add(svec_t *, const char *);
void svec_add_nocopy(svec_t *, char *);
void svec_del(svec_t *, const char *);
void svec_append(svec_t *, const svec_t *);
void svec_terminate(svec_t *);
void svec_sort(svec_t *);
void svec_sort_unique(svec_t *);
void svec_unique(svec_t *);
void svec_compact(svec_t *);
void svec_shuffle(svec_t *);
void svec_diff(const svec_t *a, const svec_t *b,
               svec_t *a_only, svec_t *both, svec_t *b_only);
bool svec_contains(const svec_t *, const char *);
size_t svec_find(const svec_t *, const char *);
bool svec_is_sorted(const svec_t *);
bool svec_is_unique(const svec_t *);
const char *svec_get_duplicate(const svec_t *);
void svec_swap(svec_t *a, svec_t *b);
void svec_print(const svec_t *svec, const char *title);
//void svec_parse_words(svec_t *svec, const char *words);
bool svec_equal(const svec_t *, const svec_t *);
char *svec_join(const svec_t *,
                const char *delimiter, const char *terminator);
const char *svec_back(const svec_t *);
void svec_pop_back(svec_t *);

/* Iterates over the names in SVEC, assigning each name in turn to NAME and its
 * index to INDEX. */
#define SVEC_FOR_EACH(INDEX, NAME, SVEC)        \
    for ((INDEX) = 0;                           \
         ((INDEX) < (SVEC)->n                   \
          ? (NAME) = (SVEC)->names[INDEX], 1    \
          : 0);                                 \
         (INDEX)++)

#ifdef  __cplusplus
}
#endif

#endif /* svec.h */
