/*
 * Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013, 2015, 2016 Nicira, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-1.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OPENLIBC_DYNAMIC_STRING_H
#define OPENLIBC_DYNAMIC_STRING_H 1

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

/* A "dynamic string", that is, a buffer that can be used to construct a
 * string across a series of operations that extend or modify it.
 *
 * The 'string' member does not always point to a null-terminated string.
 * Initially it is NULL, and even when it is nonnull, some operations do not
 * ensure that it is null-terminated.  Use ds_cstr() to ensure that memory is
 * allocated for the string and that it is null-terminated. */
typedef struct ds {
    char *string;       /* Null-terminated string. */
    size_t length;      /* Bytes used, not including null terminator. */
    size_t allocated;   /* Bytes allocated, not including null terminator. */
} ds_t;

#define DS_EMPTY_INITIALIZER { NULL, 0, 0 }

void ds_init(ds_t *);
void ds_clear(ds_t *);
void ds_truncate(ds_t *, size_t new_length);
void ds_reserve(ds_t *, size_t min_length);
char *ds_put_uninit(ds_t *, size_t n);
static inline void ds_put_char(ds_t *, char);
void ds_put_utf8(ds_t *, int uc);
void ds_put_char_multiple(ds_t *, char, size_t n);
void ds_put_buffer(ds_t *, const char *, size_t n);
void ds_put_cstr(ds_t *, const char *);
void ds_put_and_free_cstr(ds_t *, char *);
void ds_put_format(ds_t *, const char *, ...) OLC_PRINTF_FORMAT(2, 3);
void ds_put_format_valist(ds_t *, const char *, va_list)
    OLC_PRINTF_FORMAT(2, 0);
void ds_put_printable(ds_t *, const char *, size_t);
void ds_put_hex(ds_t *ds, const void *buf, size_t size);
void ds_put_hex_dump(ds_t *ds, const void *buf_, size_t size,
                     uintptr_t ofs, bool ascii);
int ds_get_line(ds_t *, FILE *);
int ds_get_preprocessed_line(ds_t *, FILE *, int *line_number);
int ds_get_test_line(ds_t *, FILE *);

void ds_put_strftime_msec(ds_t *, const char *format, long long int when,
                          bool utc);

char *ds_cstr(ds_t *);
const char *ds_cstr_ro(const ds_t *);
char *ds_steal_cstr(ds_t *);
void ds_destroy(ds_t *);
void ds_swap(ds_t *, ds_t *);

int ds_last(const ds_t *);
bool ds_chomp(ds_t *, int c);
void ds_clone(ds_t *, ds_t *);

/* Inline functions. */

void ds_put_char__(ds_t *, char);

static inline void
ds_put_char(ds_t *ds, char c)
{
    if (ds->length < ds->allocated) {
        ds->string[ds->length++] = c;
        ds->string[ds->length] = '\0';
    } else {
        ds_put_char__(ds, c);
    }
}

#ifdef __cplusplus
}
#endif

#endif /* dynamic-string.h */
