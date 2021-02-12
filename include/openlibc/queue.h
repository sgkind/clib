/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#ifndef OPENLIBC_QUEUE_H
#define OPENLIBC_QUEUE_H 1

#include "openlibc/types.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct olc_queue_s  olc_queue_t;

struct olc_queue_s {
    olc_queue_t *prev;
    olc_queue_t *next;
};


#define olc_queue_init(q)                                                     \
    (q)->prev = q;                                                            \
    (q)->next = q


#define olc_queue_empty(h)                                                    \
    (h == (h)->prev)


#define olc_queue_insert_head(h, x)                                           \
    (x)->next = (h)->next;                                                    \
    (x)->next->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->next = x


#define olc_queue_insert_after  olc_queue_insert_head


#define olc_queue_insert_tail(h, x)                                           \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->next = x;                                                      \
    (x)->next = h;                                                            \
    (h)->prev = x


#define olc_queue_head(h)                                                     \
    (h)->next


#define olc_queue_last(h)                                                     \
    (h)->prev


#define olc_queue_sentinel(h)                                                 \
    (h)


#define olc_queue_next(q)                                                     \
    (q)->next


#define olc_queue_prev(q)                                                     \
    (q)->prev


#if (OLC_DEBUG)

#define olc_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next;                                              \
    (x)->prev = NULL;                                                         \
    (x)->next = NULL

#else

#define olc_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next

#endif


#define olc_queue_split(h, q, n)                                              \
    (n)->prev = (h)->prev;                                                    \
    (n)->prev->next = n;                                                      \
    (n)->next = q;                                                            \
    (h)->prev = (q)->prev;                                                    \
    (h)->prev->next = h;                                                      \
    (q)->prev = n


#define olc_queue_add(h, n)                                                   \
    (h)->prev->next = (n)->next;                                              \
    (n)->next->prev = (h)->prev;                                              \
    (h)->prev = (n)->prev;                                                    \
    (h)->prev->next = h


#define olc_queue_data(q, type, link)                                         \
    (type *) ((u_char *) q - offsetof(type, link))


olc_queue_t *olc_queue_middle(olc_queue_t *queue);
void olc_queue_sort(olc_queue_t *queue,
    olc_int_t (*cmp)(const olc_queue_t *, const olc_queue_t *));


#ifdef __cplusplus
}
#endif

#endif /* OPENLIBC_QUEUE_H */