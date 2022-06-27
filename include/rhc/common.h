#ifndef RHC_COMMON_H
#define RHC_COMMON_H

//
// some default includes, types and macros to use
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

//
// Definitions
//

typedef uint8_t rhcu8;
typedef int8_t rhci8;
typedef uint16_t rhcu16;
typedef int16_t rhci16;
typedef uint32_t rhcu32;
typedef int32_t rhci32;
typedef uint64_t rhcu64;
typedef int64_t rhci64;
typedef float rhcf32;
typedef double rhcf64;
typedef rhci64 rhcsize;

#define rhc_min(a, b) ((a)<(b)?(a):(b))
#define rhc_max(a, b) ((a)>(b)?(a):(b))


//
// windows stuff
//

#ifdef PLATFORM_MSVC
#define _Thread_local __declspec(thread)
#endif

#endif //RHC_COMMON_H
