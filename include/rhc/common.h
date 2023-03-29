#ifndef RHC_COMMON_H
#define RHC_COMMON_H

//
// some default includes, types and macros to use
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
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

#define rhc_min(a, b) ((a)<(b)?(a):(b))
#define rhc_max(a, b) ((a)>(b)?(a):(b))

#define rhc_sign(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))
#define rhc_clamp(x, min, max) (x) < (min) ? (min) : ((x) > (max) ? (max) : (x))
#define rhc_step(x, edge) (x) < (edge) ? 0 : 1;


// unpack arrays (and matrices 3x3=9, 4x4=16)
#define RHC_UP2(v) (v)[0], (v)[1]
#define RHC_UP3(v) (v)[0], (v)[1], (v)[2]
#define RHC_UP4(v) (v)[0], (v)[1], (v)[2], (v)[3]
#define RHC_UP9(v) (v)[0], (v)[1], (v)[2], (v)[3], (v)[4], (v)[5], (v)[6], (v)[7], (v)[8]
#define RHC_UP16(v) (v)[0], (v)[1], (v)[2], (v)[3], (v)[4], (v)[5], (v)[6], (v)[7], \
(v)[8], (v)[9], (v)[10], (v)[11], (v)[12], (v)[13], (v)[14], (v)[15]

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

#define RHC_U8_MIN 0
#define RHC_U8_MAX 255
#define RHC_I8_MIN (-128)
#define RHC_I8_MAX 127
#define RHC_U16_MIN 0
#define RHC_U16_MAX 65535
#define RHC_I16_MIN (-32768)
#define RHC_I16_MAX 32767
#define RHC_U32_MIN 0
#define RHC_U32_MAX 4294967295
#define RHC_I32_MIN (-2147483648)
#define RHC_I32_MAX 2147483647
#define RHC_U64_MIN 0
#define RHC_U64_MAX 18446744073709551615UL
#define RHC_I64_MIN (-9223372036854775808LL)
#define RHC_I64_MAX 9223372036854775807LL
#define RHC_SIZE_MIN (-9223372036854775808LL)
#define RHC_SIZE_MAX 9223372036854775807LL

//
// windows stuff
//

#ifdef PLATFORM_MSVC
#define _Thread_local __declspec(thread)
#endif

#endif //RHC_COMMON_H
