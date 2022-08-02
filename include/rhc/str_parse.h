#ifndef RHC_STR_PARSE_H
#define RHC_STR_PARSE_H

//
// special str functions to read and write to little or big endian
//

#include "endian.h"
#include "str.h"

//
// ascii
//

// eats the next (with leading spaces) int_x_t and returns the str without it.
#define RHC__STR_PARSE_HELPER_EAT_IX_ASCII_PROTOTYPE_(x)\
static RhcStr_s rhc_str_eat_i ## x ## _ascii(RhcStr_s s, rhci ## x *opt_eaten) {\
    if(rhc_str_empty(s))\
        return s;\
    char *end;\
    long long int res = strtoll(s.data, &end, 0);\
    rhcsize ate_size = end - s.data;\
    if(ate_size <= 0 || ate_size > s.size) {\
        rhc_error_set("rhc_str_eat_int_ascii failed");\
        rhc_log_warn("rhc_str_eat_i" #x "_ascii: failed");\
        return rhc_str_new_invalid();\
    }\
    if(opt_eaten)\
        *opt_eaten = (rhci ## x) res;\
    return (RhcStr_s) {s.data+ate_size, s.size-ate_size};\
}

RHC__STR_PARSE_HELPER_EAT_IX_ASCII_PROTOTYPE_(8)

RHC__STR_PARSE_HELPER_EAT_IX_ASCII_PROTOTYPE_(16)

RHC__STR_PARSE_HELPER_EAT_IX_ASCII_PROTOTYPE_(32)

RHC__STR_PARSE_HELPER_EAT_IX_ASCII_PROTOTYPE_(64)


// eats the next (with leading spaces) uint_x_t and returns the str without it.
#define RHC__STR_PARSE_HELPER_EAT_UX_ASCII_PROTOTYPE_(x)\
static RhcStr_s rhc_str_eat_u ## x ## _ascii(RhcStr_s s, rhcu ## x *opt_eaten) {\
    if(rhc_str_empty(s))\
        return s;\
    char *end;\
    unsigned long long int res = strtoull(s.data, &end, 0);\
    rhcsize ate_size = end - s.data;\
    if(ate_size <= 0 || ate_size > s.size) {\
        rhc_error_set("rhc_str_eat_uint_ascii failed");\
        rhc_log_warn("rhc_str_eat_u" #x "_ascii: failed");\
        return rhc_str_new_invalid();\
    }\
    if(opt_eaten)\
        *opt_eaten = (rhcu ## x ) res;\
    return (RhcStr_s) {s.data+ate_size, s.size-ate_size};\
}

RHC__STR_PARSE_HELPER_EAT_UX_ASCII_PROTOTYPE_(8)

RHC__STR_PARSE_HELPER_EAT_UX_ASCII_PROTOTYPE_(16)

RHC__STR_PARSE_HELPER_EAT_UX_ASCII_PROTOTYPE_(32)

RHC__STR_PARSE_HELPER_EAT_UX_ASCII_PROTOTYPE_(64)


// eats the next (with leading spaces) f32 and returns the str without it.
static RhcStr_s rhc_str_eat_f32_ascii(RhcStr_s s, rhcf32 *opt_eaten) {
    if (rhc_str_empty(s))
        return s;
    char *end;
    rhcf64 res = strtod(s.data, &end);
    rhcsize ate_size = end - s.data;
    if (ate_size <= 0 || ate_size > s.size) {
        rhc_error_set("rhc_str_eat_f32_ascii failed");
        rhc_log_warn("rhc_str_eat_f32_ascii: failed");
        return rhc_str_new_invalid();
    }
    if (opt_eaten)
        *opt_eaten = (rhcf32) res;
    return (RhcStr_s) {s.data + ate_size, s.size - ate_size};
}

// eats the next (with leading spaces) f64 and returns the str without it.
static RhcStr_s rhc_str_eat_f64_ascii(RhcStr_s s, rhcf64 *opt_eaten) {
    if (rhc_str_empty(s))
        return s;
    char *end;
    rhcf64 res = strtod(s.data, &end);
    rhcsize ate_size = end - s.data;
    if (ate_size <= 0 || ate_size > s.size) {
        rhc_error_set("rhc_str_eat_f64_ascii failed");
        rhc_log_warn("rhc_str_eat_f64_ascii: failed");
        return rhc_str_new_invalid();
    }
    if (opt_eaten)
        *opt_eaten = (rhcf64) res;
    return (RhcStr_s) {s.data + ate_size, s.size - ate_size};
}


//
// binary
//

// eats the next intx_t as binary little endian and returns the str without it.
#define RHC__STR_PARSE_HELPER_EAT_IX_BINARY_LE_PROTOTPYE_(x)\
static RhcStr_s rhc_str_eat_i ## x ## _binary_le(RhcStr_s s, rhci ## x *opt_eaten) {\
    if(rhc_str_empty(s))\
        return s;\
    if(s.size < (x)/8) {\
        rhc_error_set("rhc_str_eat_int_binary_le failed");\
        rhc_log_warn("rhc_str_eat_i" #x "_binary_le: failed");\
        return rhc_str_new_invalid();\
    }\
    if(opt_eaten){\
        memcpy(opt_eaten, s.data, (x)/8);\
        if(!rhc_endian_system_is_binary_little_endian())\
            rhc_endian_swap(opt_eaten, (x)/8);\
    }\
    s.data += (x)/8;\
    s.size -= (x)/8;\
    return s;\
}

RHC__STR_PARSE_HELPER_EAT_IX_BINARY_LE_PROTOTPYE_(8)

RHC__STR_PARSE_HELPER_EAT_IX_BINARY_LE_PROTOTPYE_(16)

RHC__STR_PARSE_HELPER_EAT_IX_BINARY_LE_PROTOTPYE_(32)

RHC__STR_PARSE_HELPER_EAT_IX_BINARY_LE_PROTOTPYE_(64)

// eats the next intx_t as binary big endian and returns the str without it.
#define RHC__STR_PARSE_HELPER_EAT_IX_BINARY_BE_PROTOTPYE_(x)\
static RhcStr_s rhc_str_eat_i ## x ## _binary_be(RhcStr_s s, rhci ## x *opt_eaten) {\
    if(rhc_str_empty(s))\
        return s;\
    if(s.size < (x)/8) {\
        rhc_error_set("rhc_str_eat_int_binary_be failed");\
        rhc_log_warn("rhc_str_eat_i" #x "_binary_be: failed");\
        return rhc_str_new_invalid();\
    }\
    if(opt_eaten){\
        memcpy(opt_eaten, s.data, (x)/8);\
        if(rhc_endian_system_is_binary_little_endian())\
            rhc_endian_swap(opt_eaten, (x)/8);\
    }\
    s.data += (x)/8;\
    s.size -= (x)/8;\
    return s;\
}

RHC__STR_PARSE_HELPER_EAT_IX_BINARY_BE_PROTOTPYE_(8)

RHC__STR_PARSE_HELPER_EAT_IX_BINARY_BE_PROTOTPYE_(16)

RHC__STR_PARSE_HELPER_EAT_IX_BINARY_BE_PROTOTPYE_(32)

RHC__STR_PARSE_HELPER_EAT_IX_BINARY_BE_PROTOTPYE_(64)

// eats the next uintx_t as binary little endian and returns the str without it.
#define RHC__STR_PARSE_HELPER_EAT_UX_BINARY_LE_PROTOTPYE_(x)\
static RhcStr_s rhc_str_eat_u ## x ## _binary_le(RhcStr_s s, rhcu ## x *opt_eaten) {\
    if(rhc_str_empty(s))\
        return s;\
    if(s.size < (x)/8) {\
        rhc_error_set("rhc_str_eat_uint_binary_le failed");\
        rhc_log_warn("rhc_str_eat_u" #x "_binary_le: failed");\
        return rhc_str_new_invalid();\
    }\
    if(opt_eaten){\
        memcpy(opt_eaten, s.data, (x)/8);\
        if(!rhc_endian_system_is_binary_little_endian())\
            rhc_endian_swap(opt_eaten, (x)/8);\
    }\
    s.data += (x)/8;\
    s.size -= (x)/8;\
    return s;\
}

RHC__STR_PARSE_HELPER_EAT_UX_BINARY_LE_PROTOTPYE_(8)

RHC__STR_PARSE_HELPER_EAT_UX_BINARY_LE_PROTOTPYE_(16)

RHC__STR_PARSE_HELPER_EAT_UX_BINARY_LE_PROTOTPYE_(32)

RHC__STR_PARSE_HELPER_EAT_UX_BINARY_LE_PROTOTPYE_(64)

// eats the next uintx_t as binary big endian and returns the str without it.
#define RHC__STR_PARSE_HELPER_EAT_UX_BINARY_BE_PROTOTPYE_(x)\
static RhcStr_s rhc_str_eat_u ## x ## _binary_be(RhcStr_s s, rhcu ## x *opt_eaten) {\
    if(rhc_str_empty(s))\
        return s;\
    if(s.size < (x)/8) {\
        rhc_error_set("rhc_str_eat_uint_binary_be failed");\
        rhc_log_warn("rhc_str_eat_u" #x "_binary_be: failed");\
        return rhc_str_new_invalid();\
    }\
    if(opt_eaten){\
        memcpy(opt_eaten, s.data, (x)/8);\
        if(rhc_endian_system_is_binary_little_endian())\
            rhc_endian_swap(opt_eaten, (x)/8);\
    }\
    s.data += (x)/8;\
    s.size -= (x)/8;\
    return s;\
}

RHC__STR_PARSE_HELPER_EAT_UX_BINARY_BE_PROTOTPYE_(8)

RHC__STR_PARSE_HELPER_EAT_UX_BINARY_BE_PROTOTPYE_(16)

RHC__STR_PARSE_HELPER_EAT_UX_BINARY_BE_PROTOTPYE_(32)

RHC__STR_PARSE_HELPER_EAT_UX_BINARY_BE_PROTOTPYE_(64)


// eats the next f32 as binary little endian and returns the str without it.
static RhcStr_s rhc_str_eat_f32_binary_le(RhcStr_s s, rhcf32 *opt_eaten) {
    if (rhc_str_empty(s))
        return s;
    if (s.size < 4) {
        rhc_error_set("rhc_str_eat_f32_binary_le failed");
        rhc_log_warn("rhc_str_eat_f32_binary_le: failed");
        return rhc_str_new_invalid();
    }
    if (opt_eaten) {
        memcpy(opt_eaten, s.data, 4);
        if (!rhc_endian_system_is_binary_little_endian())
            rhc_endian_swap(opt_eaten, 4);
    }
    s.data += 4;
    s.size -= 4;
    return s;
}

// eats the next f32 as binary big endian and returns the str without it.
static RhcStr_s rhc_str_eat_f32_binary_be(RhcStr_s s, rhcf32 *opt_eaten) {
    if (rhc_str_empty(s))
        return s;
    if (s.size < 4) {
        rhc_error_set("rhc_str_eat_f32_binary_be failed");
        rhc_log_warn("rhc_str_eat_f32_binary_be: failed");
        return rhc_str_new_invalid();
    }
    if (opt_eaten) {
        memcpy(opt_eaten, s.data, 4);
        if (rhc_endian_system_is_binary_little_endian())
            rhc_endian_swap(opt_eaten, 4);
    }
    s.data += 4;
    s.size -= 4;
    return s;
}


// eats the next f64 as binary little endian and returns the str without it.
static RhcStr_s rhc_str_eat_f64_binary_le(RhcStr_s s, rhcf64 *opt_eaten) {
    if (rhc_str_empty(s))
        return s;
    if (s.size < 8) {
        rhc_error_set("rhc_str_eat_f64_binary_le failed");
        rhc_log_warn("rhc_str_eat_f64_binary_le: failed");
        return rhc_str_new_invalid();
    }
    if (opt_eaten) {
        memcpy(opt_eaten, s.data, 8);
        if (!rhc_endian_system_is_binary_little_endian())
            rhc_endian_swap(opt_eaten, 8);
    }
    s.data += 8;
    s.size -= 8;
    return s;
}

// eats the next f64 as binary big endian and returns the str without it.
static RhcStr_s rhc_str_eat_f64_binary_be(RhcStr_s s, rhcf64 *opt_eaten) {
    if (rhc_str_empty(s))
        return s;
    if (s.size < 8) {
        rhc_error_set("rhc_str_eat_f64_binary_be failed");
        rhc_log_warn("rhc_str_eat_f64_binary_be: failed");
        return rhc_str_new_invalid();
    }
    if (opt_eaten) {
        memcpy(opt_eaten, s.data, 8);
        if (rhc_endian_system_is_binary_little_endian())
            rhc_endian_swap(opt_eaten, 8);
    }
    s.data += 8;
    s.size -= 8;
    return s;
}

//
// feed binary
//

// feeds an intx_t as binary little endian and returns the str without it (behind it).
#define RHC__STR_PARSE_HELPER_FEED_IX_BINARY_LE_PROTOTPYE_(x)\
static RhcStr_s rhc_str_feed_int ## x ## _binary_le(RhcStr_s s, rhci ## x feed) {\
    if(rhc_str_empty(s))\
        return s;\
    if(s.size < (x)/8) {\
        rhc_error_set("rhc_str_feed_int_binary_le failed");\
        rhc_log_warn("rhc_str_feed_i" #x "_binary_le: failed");\
        return rhc_str_new_invalid();\
    }\
    if(!rhc_endian_system_is_binary_little_endian())\
        rhc_endian_swap(&feed, (x)/8);\
    memcpy(s.data, &feed, (x)/8);\
    s.data += (x)/8;\
    s.size -= (x)/8;\
    return s;\
}

RHC__STR_PARSE_HELPER_FEED_IX_BINARY_LE_PROTOTPYE_(8)

RHC__STR_PARSE_HELPER_FEED_IX_BINARY_LE_PROTOTPYE_(16)

RHC__STR_PARSE_HELPER_FEED_IX_BINARY_LE_PROTOTPYE_(32)

RHC__STR_PARSE_HELPER_FEED_IX_BINARY_LE_PROTOTPYE_(64)

// feeds an intx_t as binary bigb endian and returns the str without it (behind it).
#define RHC__STR_PARSE_HELPER_FEED_IX_BINARY_BE_PROTOTPYE_(x)\
static RhcStr_s rhc_str_feed_int ## x ## _binary_be(RhcStr_s s, rhci ## x feed) {\
    if(rhc_str_empty(s))\
        return s;\
    if(s.size < (x)/8) {\
        rhc_error_set("rhc_str_feed_int_binary_be failed");\
        rhc_log_warn("rhc_str_feed_i" #x "_binary_be: failed");\
        return rhc_str_new_invalid();\
    }\
    if(rhc_endian_system_is_binary_little_endian())\
        rhc_endian_swap(&feed, (x)/8);\
    memcpy(s.data, &feed, (x)/8);\
    s.data += (x)/8;\
    s.size -= (x)/8;\
    return s;\
}

RHC__STR_PARSE_HELPER_FEED_IX_BINARY_BE_PROTOTPYE_(8)

RHC__STR_PARSE_HELPER_FEED_IX_BINARY_BE_PROTOTPYE_(16)

RHC__STR_PARSE_HELPER_FEED_IX_BINARY_BE_PROTOTPYE_(32)

RHC__STR_PARSE_HELPER_FEED_IX_BINARY_BE_PROTOTPYE_(64)

// feeds an uintx_t as binary little endian and returns the str without it (behind it).
#define RHC__STR_PARSE_HELPER_FEED_UX_BINARY_LE_PROTOTPYE_(x)\
static RhcStr_s rhc_str_feed_uint ## x ## _binary_le(RhcStr_s s, rhcu ## x feed) {\
    if(rhc_str_empty(s))\
        return s;\
    if(s.size < (x)/8) {\
        rhc_error_set("rhc_str_feed_uint_binary_le failed");\
        rhc_log_warn("rhc_str_feed_u" #x "_binary_le: failed");\
        return rhc_str_new_invalid();\
    }\
    if(!rhc_endian_system_is_binary_little_endian())\
        rhc_endian_swap(&feed, (x)/8);\
    memcpy(s.data, &feed, (x)/8);\
    s.data += (x)/8;\
    s.size -= (x)/8;\
    return s;\
}

RHC__STR_PARSE_HELPER_FEED_UX_BINARY_LE_PROTOTPYE_(8)

RHC__STR_PARSE_HELPER_FEED_UX_BINARY_LE_PROTOTPYE_(16)

RHC__STR_PARSE_HELPER_FEED_UX_BINARY_LE_PROTOTPYE_(32)

RHC__STR_PARSE_HELPER_FEED_UX_BINARY_LE_PROTOTPYE_(64)

// feeds an uintx_t as binary bigb endian and returns the str without it (behind it).
#define RHC__STR_PARSE_HELPER_FEED_UX_BINARY_BE_PROTOTPYE_(x)\
static RhcStr_s rhc_str_feed_uint ## x ## _binary_be(RhcStr_s s, rhcu ## x  feed) {\
    if(rhc_str_empty(s))\
        return s;\
    if(s.size < (x)/8) {\
        rhc_error_set("rhc_str_feed_uint_binary_be failed");\
        rhc_log_warn("rhc_str_feed_u" #x "_binary_be: failed");\
        return rhc_str_new_invalid();\
    }\
    if(rhc_endian_system_is_binary_little_endian())\
        rhc_endian_swap(&feed, (x)/8);\
    memcpy(s.data, &feed, (x)/8);\
    s.data += (x)/8;\
    s.size -= (x)/8;\
    return s;\
}

RHC__STR_PARSE_HELPER_FEED_UX_BINARY_BE_PROTOTPYE_(8)

RHC__STR_PARSE_HELPER_FEED_UX_BINARY_BE_PROTOTPYE_(16)

RHC__STR_PARSE_HELPER_FEED_UX_BINARY_BE_PROTOTPYE_(32)

RHC__STR_PARSE_HELPER_FEED_UX_BINARY_BE_PROTOTPYE_(64)


// feeds a f32 as binary little endian and returns the str without it (behind it).
static RhcStr_s rhc_str_feed_f32_binary_le(RhcStr_s s, rhcf32 feed) {
    if (rhc_str_empty(s))
        return s;
    if (s.size < 4) {
        rhc_error_set("rhc_str_feed_f32_binary_le failed");
        rhc_log_warn("rhc_str_feed_f32_binary_le: failed");
        return rhc_str_new_invalid();
    }
    if (!rhc_endian_system_is_binary_little_endian())
        rhc_endian_swap(&feed, 4);
    memcpy(s.data, &feed, 4);
    s.data += 4;
    s.size -= 4;
    return s;
}

// feeds a f32 as binary little endian and returns the str without it (behind it).
static RhcStr_s rhc_str_feed_f32_binary_be(RhcStr_s s, rhcf32 feed) {
    if (rhc_str_empty(s))
        return s;
    if (s.size < 4) {
        rhc_error_set("rhc_str_feed_f32_binary_be failed");
        rhc_log_warn("rhc_str_feed_f32_binary_be: failed");
        return rhc_str_new_invalid();
    }
    if (rhc_endian_system_is_binary_little_endian())
        rhc_endian_swap(&feed, 4);
    memcpy(s.data, &feed, 4);
    s.data += 4;
    s.size -= 4;
    return s;
}

// feeds a f64 as binary little endian and returns the str without it (behind it).
static RhcStr_s rhc_str_feed_f64_binary_le(RhcStr_s s, rhcf64 feed) {
    if (rhc_str_empty(s))
        return s;
    if (s.size < 8) {
        rhc_error_set("rhc_str_feed_f64_binary_le failed");
        rhc_log_warn("rhc_str_feed_f64_binary_le: failed");
        return rhc_str_new_invalid();
    }
    if (!rhc_endian_system_is_binary_little_endian())
        rhc_endian_swap(&feed, 8);
    memcpy(s.data, &feed, 8);
    s.data += 8;
    s.size -= 8;
    return s;
}

// feeds a f64 as binary little endian and returns the str without it (behind it).
static RhcStr_s rhc_str_feed_f64_binary_be(RhcStr_s s, rhcf64 feed) {
    if (rhc_str_empty(s))
        return s;
    if (s.size < 8) {
        rhc_error_set("rhc_str_feed_f64_binary_be failed");
        rhc_log_warn("rhc_str_feed_f64_binary_be: failed");
        return rhc_str_new_invalid();
    }
    if (rhc_endian_system_is_binary_little_endian())
        rhc_endian_swap(&feed, 8);
    memcpy(s.data, &feed, 8);
    s.data += 8;
    s.size -= 8;
    return s;
}

#endif //RHC_STR_PARSE_H
