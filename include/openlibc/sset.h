/*
 * Copyright (c) 2011, 2012, 2013, 2015, 2016 Nicira, Inc.
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

#ifndef OPENLIBC_SSET_H
#define OPENLIBC_SSET_H 1

#include "openlibc/hmap.h"
#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif

struct sset_node {
    struct hmap_node hmap_node;
    char name[1];
};

/* A set of strings. */
typedef struct sset {
    struct hmap map;
} sset_t;

#define SSET_INITIALIZER(SSET) { HMAP_INITIALIZER(&(SSET)->map) }

/* Basics. */
void sset_init(sset_t *);
void sset_destroy(sset_t *);
void sset_clone(sset_t *, const sset_t *);
void sset_swap(sset_t *, sset_t *);
void sset_moved(sset_t *);

/* String parsing and formatting. */
void sset_from_delimited_string(sset_t *, const char *s,
                                const char *delimiters);
char *sset_join(const sset_t *,
                const char *delimiter, const char *terminator);

/* Count. */
bool sset_is_empty(const sset_t *);
size_t sset_count(const sset_t *);

/* Insertion. */
struct sset_node *sset_add(sset_t *, const char *);
struct sset_node *sset_add_and_free(sset_t *, char *);
void sset_add_assert(sset_t *, const char *);
void sset_add_array(sset_t *, char **, size_t n);

/* Deletion. */
void sset_clear(sset_t *);
void sset_delete(sset_t *, struct sset_node *);
bool sset_find_and_delete(sset_t *, const char *);
void sset_find_and_delete_assert(sset_t *, const char *);
char *sset_pop(sset_t *);

/* Search. */
struct sset_node *sset_find(const sset_t *, const char *);
bool sset_contains(const sset_t *, const char *);
bool sset_equals(const sset_t *, const sset_t *);

struct sset_position {
    struct hmap_position pos;
};

struct sset_node *sset_at_position(const sset_t *,
                                   struct sset_position *);

/* Set operations. */
void sset_intersect(sset_t *, const sset_t *);

/* Iteration macros. */
#define SSET_FOR_EACH(NAME, SSET)               \
    for ((NAME) = SSET_FIRST(SSET);             \
         NAME != NULL;                          \
         (NAME) = SSET_NEXT(SSET, NAME))

#define SSET_FOR_EACH_SAFE(NAME, NEXT, SSET)        \
    for ((NAME) = SSET_FIRST(SSET);                 \
         (NAME != NULL                              \
          ? (NEXT) = SSET_NEXT(SSET, NAME), true    \
          : false);                                 \
         (NAME) = (NEXT))

const char **sset_array(const sset_t *);
const char **sset_sort(const sset_t *);

/* Implementation helper macros. */

#define SSET_NODE_FROM_HMAP_NODE(HMAP_NODE) \
    CONTAINER_OF(HMAP_NODE, struct sset_node, hmap_node)
#define SSET_NAME_FROM_HMAP_NODE(HMAP_NODE) \
    HMAP_NODE == NULL                       \
    ? NULL                                  \
    : (CONST_CAST(const char *, (SSET_NODE_FROM_HMAP_NODE(HMAP_NODE)->name)))
#define SSET_NODE_FROM_NAME(NAME) CONTAINER_OF(NAME, struct sset_node, name)
#define SSET_FIRST(SSET)                                    \
    (BUILD_ASSERT_TYPE(SSET, sset_t *),                \
     SSET_NAME_FROM_HMAP_NODE(hmap_first(&(SSET)->map)))
#define SSET_NEXT(SSET, NAME)                                           \
    (BUILD_ASSERT_TYPE(SSET, sset_t *),                            \
     SSET_NAME_FROM_HMAP_NODE(                                          \
         hmap_next(&(SSET)->map, &SSET_NODE_FROM_NAME(NAME)->hmap_node)))

#ifdef __cplusplus
}
#endif

#endif /* sset.h */