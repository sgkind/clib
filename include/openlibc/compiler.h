/*
 * Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2016, 2017 Nicira, Inc.
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

#ifndef OPENLIBC_COMPILER_H
#define OPENLIBC_COMPILER_H 1

#include <stddef.h>
#include <stdbool.h>

#ifndef __has_feature
#define __has_feature(x) 0
#endif
#ifndef __has_extension
#define __has_extension(x) 0
#endif

#if __GNUC__ && !__CHECKER__
#define STRFTIME_FORMAT(FMT) __attribute__((__format__(__strftime__, FMT, 0)))
#define MALLOC_LIKE __attribute__((__malloc__))
#define ALWAYS_INLINE __attribute__((always_inline))
#define SENTINEL(N) __attribute__((sentinel(N)))
#else
#define STRFTIME_FORMAT(FMT)
#define MALLOC_LIKE
#define ALWAYS_INLINE
#define SENTINEL(N)
#endif

#if __GNUC__ && !__CHECKER__
#define OLC_UNUSED __attribute__((__unused__))
#define OLC_PRINTF_FORMAT(FMT, ARG1) __attribute__((__format__(printf, FMT, ARG1)))
#define OLC_SCANF_FORMAT(FMT, ARG1) __attribute__((__format__(scanf, FMT, ARG1)))
#define OLC_WARN_UNUSED_RESULT __attribute__((__warn_unused_result__))
#define OLC_LIKELY(CONDITION) __builtin_expect(!!(CONDITION), 1)
#define OLC_UNLIKELY(CONDITION) __builtin_expect(!!(CONDITION), 0)
#else
#define OLC_UNUSED
#define OLC_PRINTF_FORMAT(FMT, ARG1)
#define OLC_SCANF_FORMAT(FMT, ARG1)
#define OLC_WARN_UNUSED_RESULT
#define OLC_LIKELY(CONDITION) (!!(CONDITION))
#define OLC_UNLIKELY(CONDITION) (!!(CONDITION))
#endif

/* Build assertions.
 *
 * Use BUILD_ASSERT_DECL as a declaration or a statement, or BUILD_ASSERT as
 * part of an expression. */
#ifdef __CHECKER__
#define BUILD_ASSERT(EXPR) ((void) 0)
#define BUILD_ASSERT_DECL(EXPR) extern int (*build_assert(void))[1]
#elif defined(__cplusplus) && __cplusplus >= 201103L
#define BUILD_ASSERT(EXPR) static_assert(EXPR, "assertion failed")
#define BUILD_ASSERT_DECL(EXPR) static_assert(EXPR, "assertion failed")
#elif defined(__cplusplus) && __cplusplus < 201103L
#include <boost/static_assert.hpp>
#define BUILD_ASSERT BOOST_STATIC_ASSERT
#define BUILD_ASSERT_DECL BOOST_STATIC_ASSERT
#elif (__GNUC__ * 256 + __GNUC_MINOR__ >= 0x403 \
       || __has_extension(c_static_assert))
#define BUILD_ASSERT_DECL(EXPR) _Static_assert(EXPR, #EXPR)
#define BUILD_ASSERT(EXPR) (void) ({ _Static_assert(EXPR, #EXPR); })
#else
#define BUILD_ASSERT__(EXPR) \
        sizeof(struct { unsigned int build_assert_failed : (EXPR) ? 1 : -1; })
#define BUILD_ASSERT(EXPR) (void) BUILD_ASSERT__(EXPR)
#define BUILD_ASSERT_DECL(EXPR) \
        extern int (*build_assert(void))[BUILD_ASSERT__(EXPR)]
#endif

#ifdef __GNUC__
#define BUILD_ASSERT_GCCONLY(EXPR) BUILD_ASSERT(EXPR)
#define BUILD_ASSERT_DECL_GCCONLY(EXPR) BUILD_ASSERT_DECL(EXPR)
#else
#define BUILD_ASSERT_GCCONLY(EXPR) ((void) 0)
#define BUILD_ASSERT_DECL_GCCONLY(EXPR) ((void) 0)
#endif


#endif /* compiler.h */