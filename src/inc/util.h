#ifndef UTIL_H
#define UTIL_H 1

#include <stdlib.h>
#include <string.h>

#include "openlibc/compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

void *xmalloc(size_t) MALLOC_LIKE;
void *xrealloc(void *p, size_t);
void *xmemdup(const void *, size_t) MALLOC_LIKE;
char *xmemdup0(const char *, size_t) MALLOC_LIKE;
void *x2nrealloc(void *p, size_t *n, size_t s);

/* The C standards say that neither the 'dst' nor 'src' argument to
 * memcpy() may be null, even if 'n' is zero.  This wrapper tolerates
 * the null case. */
static inline void
nullable_memcpy(void *dst, const void *src, size_t n)
{
    if (n) {
        memcpy(dst, src, n);
    }
}

/* The C standards say that the 'dst' argument to memset may not be
 * null, even if 'n' is zero.  This wrapper tolerates the null case. */
static inline void
nullable_memset(void *dst, int c, size_t n)
{
    if (n) {
        memset(dst, c, n);
    }
}

#ifdef __cplusplus
};
#endif

#endif /* util.h */