/*
 * Common type and macro definitions.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

/*
 * Fixed-width integer types.
 */

typedef signed char          int8;
typedef signed short int     int16;
typedef signed int           int32;
typedef signed long long int int64;

typedef unsigned char          uint8;
typedef unsigned short int     uint16;
typedef unsigned int           uint32;
typedef unsigned long long int uint64;

/*
 * Native-width integer types.
 */

#if CONFIG_64BIT == 1
    typedef int64  intn;
    typedef uint64 uintn;
#else
    typedef int32  intn;
    typedef uint32 uintn;
#endif

/*
 * Character types.
 */

typedef char   char8;
typedef uint16 char16;

/*
 * Boolean type.
 */

typedef uint8 bool;

#define true  ((bool) 1)
#define false ((bool) 0)

/*
 * Variable argument list.
 */

typedef __builtin_va_list VaList;

#define vaListStart __builtin_va_start
#define vaListPop   __builtin_va_arg
#define vaListEnd   __builtin_va_end

/*
 * NULL pointer constant.
 */

#define NULL ((void *) 0)

/*
 * Bitfield helpers.
 */

#define BIT(n) (1ULL << (n))

#define SET_BITS(x, b)   ((x) |= (b))
#define CLEAR_BITS(x, b) ((x) &= ~(b))

#define SET_BIT(x, b)   SET_BITS(x, b)
#define CLEAR_BIT(x, b) CLEAR_BITS(x, b)

/*
 * Compiler attributes.
 */

#define __align(n) __attribute__((__aligned__((n))))
#define __noreturn __attribute__((noreturn))
#define __packed   __attribute__((packed))
#define __sysv_abi __attribute__((sysv_abi))

/*
 * Miscellaneous.
 */

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#define ALIGN_DOWN(x, a) ((x) - ((x) & ((a) - 1)))
#define ALIGN_UP(x, a)   (((x) + ((align) - 1)) & ~((align) - 1))

#define ARRAY_LENGTH(a) (sizeof(a) / sizeof((a)[0]))

#define ASMV(...) __asm__ __volatile__(__VA_ARGS__)

#define offsetof(type, member) __builtin_offsetof(type, member)
#define unreachable()          __builtin_unreachable()
