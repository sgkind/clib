/*
 * Copyright (c) 2009, 2010, 2011, 2012 Nicira, Inc.
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

#include "openlibc/smap.h"

#include <assert.h>

#include "openlibc/hash.h"
#include "util.h"

static struct smap_node *smap_find__(const smap_t *,
                                       const char *name, size_t name_len,
                                       size_t hash);

static size_t
hash_name(const char *name)
{
    return hash_string(name, 0);
}

void
smap_init(smap_t *sh)
{
    hmap_init(&sh->map);
}

void
smap_destroy(smap_t *sh)
{
    if (sh) {
        smap_clear(sh);
        hmap_destroy(&sh->map);
    }
}

/* Like shash_destroy(), but also free() each node's 'data'. */
void
smap_destroy_free_data(smap_t *sh)
{
    if (sh) {
        smap_clear_free_data(sh);
        hmap_destroy(&sh->map);
    }
}

void
smap_swap(smap_t *a, smap_t *b)
{
    hmap_swap(&a->map, &b->map);
}

void
smap_moved(smap_t *sh)
{
    hmap_moved(&sh->map);
}

void
smap_clear(smap_t *sh)
{
    struct smap_node *node, *next;

    SMAP_FOR_EACH_SAFE (node, next, sh) {
        hmap_remove(&sh->map, &node->node);
        free(node->name);
        free(node);
    }
}

/* Like shash_clear(), but also free() each node's 'data'. */
void
smap_clear_free_data(smap_t *sh)
{
    struct smap_node *node, *next;

    SMAP_FOR_EACH_SAFE (node, next, sh) {
        hmap_remove(&sh->map, &node->node);
        free(node->data);
        free(node->name);
        free(node);
    }
}

bool
smap_is_empty(const smap_t *shash)
{
    return hmap_is_empty(&shash->map);
}

size_t
smap_count(const smap_t *shash)
{
    return hmap_count(&shash->map);
}

static struct smap_node *
smap_add_nocopy__(smap_t *sh, char *name, const void *data, size_t hash)
{
    struct smap_node *node = xmalloc(sizeof *node);
    node->name = name;
    node->data = CONST_CAST(void *, data);
    hmap_insert(&sh->map, &node->node, hash);
    return node;
}

/* It is the caller's responsibility to avoid duplicate names, if that is
 * desirable. */
struct smap_node *
smap_add_nocopy(smap_t *sh, char *name, const void *data)
{
    return smap_add_nocopy__(sh, name, data, hash_name(name));
}

/* It is the caller's responsibility to avoid duplicate names, if that is
 * desirable. */
struct smap_node *
smap_add(smap_t *sh, const char *name, const void *data)
{
    return shash_add_nocopy(sh, strdup(name), data);
}

bool
smap_add_once(smap_t *sh, const char *name, const void *data)
{
    if (!smap_find(sh, name)) {
        shash_add(sh, name, data);
        return true;
    } else {
        return false;
    }
}

void
smap_add_assert(smap_t *sh, const char *name, const void *data)
{
    assert(smap_add_once(sh, name, data));
}

/* Searches for 'name' in 'sh'.  If it does not already exist, adds it along
 * with 'data' and returns NULL.  If it does already exist, replaces its data
 * by 'data' and returns the data that it formerly contained. */
void *
smap_replace(smap_t *sh, const char *name, const void *data)
{
    size_t hash = hash_name(name);
    struct smap_node *node;

    node = smap_find__(sh, name, strlen(name), hash);
    if (!node) {
        smap_add_nocopy__(sh, strdup(name), data, hash);
        return NULL;
    } else {
        void *old_data = node->data;
        node->data = CONST_CAST(void *, data);
        return old_data;
    }
}

/* Searches for 'name' in 'sh'.  If it does not already exist, adds it along
 * with 'data' and returns NULL.  If it does already exist, replaces its data
 * by 'data' and returns the data that it formerly contained.
 *
 * Takes ownership of 'name'. */
void *
smap_replace_nocopy(smap_t *sh, char *name, const void *data)
{
    size_t hash = hash_name(name);
    struct smap_node *node;

    node = smap_find__(sh, name, strlen(name), hash);
    if (!node) {
        smap_add_nocopy__(sh, name, data, hash);
        return NULL;
    } else {
        free(name);
        void *old_data = node->data;
        node->data = CONST_CAST(void *, data);
        return old_data;
    }
}

/* Deletes 'node' from 'sh' and frees the node's name.  The caller is still
 * responsible for freeing the node's data, if necessary. */
void
smap_delete(smap_t *sh, struct smap_node *node)
{
    free(smap_steal(sh, node));
}

/* Deletes 'node' from 'sh'.  Neither the node's name nor its data is freed;
 * instead, ownership is transferred to the caller.  Returns the node's
 * name. */
char *
smap_steal(smap_t *sh, struct smap_node *node)
{
    char *name = node->name;

    hmap_remove(&sh->map, &node->node);
    free(node);
    return name;
}

static struct smap_node *
smap_find__(const smap_t *sh, const char *name, size_t name_len,
             size_t hash)
{
    struct smap_node *node;

    HMAP_FOR_EACH_WITH_HASH (node, node, hash, &sh->map) {
        if (!strncmp(node->name, name, name_len) && !node->name[name_len]) {
            return node;
        }
    }
    return NULL;
}

/* If there are duplicates, returns a random element. */
struct smap_node *
smap_find(const smap_t *sh, const char *name)
{
    return smap_find__(sh, name, strlen(name), hash_name(name));
}

/* Finds and returns a shash_node within 'sh' that has the given 'name' that is
 * exactly 'len' bytes long.  Returns NULL if no node in 'sh' has that name. */
struct smap_node *
smap_find_len(const smap_t *sh, const char *name, size_t len)
{
    return smap_find__(sh, name, len, hash_bytes(name, len, 0));
}

void *
smap_find_data(const smap_t *sh, const char *name)
{
    struct smap_node *node = smap_find(sh, name);
    return node ? node->data : NULL;
}

void *
smap_find_and_delete(smap_t *sh, const char *name)
{
    struct smap_node *node = smap_find(sh, name);
    if (node) {
        void *data = node->data;
        smap_delete(sh, node);
        return data;
    } else {
        return NULL;
    }
}

void *
smap_find_and_delete_assert(smap_t *sh, const char *name)
{
    void *data = smap_find_and_delete(sh, name);
    assert(data != NULL);
    return data;
}

struct smap_node *
smap_first(const smap_t *shash)
{
    struct hmap_node *node = hmap_first(&shash->map);
    return node ? CONTAINER_OF(node, struct smap_node, node) : NULL;
}

static int
compare_nodes_by_name(const void *a_, const void *b_)
{
    const struct smap_node *const *a = a_;
    const struct smap_node *const *b = b_;
    return strcmp((*a)->name, (*b)->name);
}

const struct smap_node **
smap_sort(const smap_t *sh)
{
    if (smap_is_empty(sh)) {
        return NULL;
    } else {
        const struct smap_node **nodes;
        struct smap_node *node;
        size_t i, n;

        n = smap_count(sh);
        nodes = xmalloc(n * sizeof *nodes);
        i = 0;
        SMAP_FOR_EACH (node, sh) {
            nodes[i++] = node;
        }
        assert(i == n);

        qsort(nodes, n, sizeof *nodes, compare_nodes_by_name);

        return nodes;
    }
}

/* Returns true if 'a' and 'b' contain the same keys (regardless of their
 * values), false otherwise. */
bool
smap_equal_keys(const smap_t *a, const smap_t *b)
{
    struct smap_node *node;

    if (hmap_count(&a->map) != hmap_count(&b->map)) {
        return false;
    }
    SMAP_FOR_EACH (node, a) {
        if (!smap_find(b, node->name)) {
            return false;
        }
    }
    return true;
}

