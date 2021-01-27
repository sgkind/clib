/*
 * Copyright (c) 2009, 2010, 2011, 2016 Nicira, Inc.
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

#ifndef SHASH_H
#define SHASH_H 1

#include "openlibc/hmap.h"
#include "openlibc/util.h"

#ifdef  __cplusplus
extern "C" {
#endif

struct smap_node {
    struct hmap_node node;
    char *name;
    void *data;
};

typedef struct shash {
    struct hmap map;
} smap_t;

#define SMAP_INITIALIZER(SMAP) { HMAP_INITIALIZER(&(SMAP)->map) }

#define SMAP_FOR_EACH(SMAP_NODE, SMAP)                               \
    HMAP_FOR_EACH_INIT (SMAP_NODE, node, &(SMAP)->map,                \
                        BUILD_ASSERT_TYPE(SMAP_NODE, struct smap_node *), \
                        BUILD_ASSERT_TYPE(SMAP, smap_t *))

#define SMAP_FOR_EACH_SAFE(SMAP_NODE, NEXT, SMAP)        \
    HMAP_FOR_EACH_SAFE_INIT (                               \
        SMAP_NODE, NEXT, node, &(SMAP)->map,              \
        BUILD_ASSERT_TYPE(SMAP_NODE, struct smap_node *), \
        BUILD_ASSERT_TYPE(NEXT, struct smap_node *),       \
        BUILD_ASSERT_TYPE(SMAP, smap_t *))

void smap_init(smap_t *);
void smap_destroy(smap_t *);
void smap_destroy_free_data(smap_t *);
void smap_swap(smap_t *, smap_t *);
void smap_moved(smap_t *);
void smap_clear(smap_t *);
void smap_clear_free_data(smap_t *);
bool smap_is_empty(const smap_t *);
size_t smap_count(const smap_t *);
struct smap_node *shash_add(smap_t *, const char *, const void *);
struct smap_node *shash_add_nocopy(smap_t *, char *, const void *);
bool smap_add_once(smap_t *, const char *, const void *);
void smap_add_assert(smap_t *, const char *, const void *);
void *smap_replace(smap_t *, const char *, const void *data);
void *smap_replace_nocopy(smap_t *, char *name, const void *data);
void smap_delete(smap_t *, struct smap_node *);
char *smap_steal(smap_t *, struct smap_node *);
struct smap_node *smap_find(const smap_t *, const char *);
struct smap_node *smap_find_len(const smap_t *, const char *, size_t);
void *smap_find_data(const smap_t *, const char *);
void *smap_find_and_delete(smap_t *, const char *);
void *smap_find_and_delete_assert(smap_t *, const char *);
struct smap_node *smap_first(const smap_t *);
const struct smap_node **smap_sort(const smap_t *);
bool smap_equal_keys(const smap_t *, const smap_t *);

#ifdef  __cplusplus
}
#endif

#endif /* shash.h */