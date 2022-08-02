#ifndef RHC_ENDIAN_H
#define RHC_ENDIAN_H

//
// host to little or big endian and vice versa for 16, 32 and 64 bit types
//      to use signed or float types, just cast these as unsinged to u*
//

#include "common.h"

// returns true if the system is little endian
static bool rhc_endian_system_is_binary_little_endian() {
    int n = 1;
    // little endian if true
    return *(char *) &n == 1;
}

// swaps endian order of the buffer
static void rhc_endian_swap(void *buf, int n) {
    assert(n <= 256 && "rhc_swap_endian: max 256 bits");
    char tmp[256];
    char *buf_data = (char *) buf;
    for (int i = 0; i < n; i++)
        tmp[i] = buf_data[n - i - 1];
    memcpy(buf_data, tmp, n);
}

//
// Contents;
// uX host to le|be
// uX le|be to host
// iX host to le|be
// iX le|be to host
//



//
// unsigned
//

static rhcu16 rhc_endian_u16_host_to_be(rhcu16 x) {
    rhcu16 result;
    rhcu8 *c;
    c = (rhcu8 *) &result;
    c[0] = (x >> 8) & 0xFF;
    c[1] = x & 0xFF;
    return result;
}

static rhcu16 rhc_endian_u16_host_to_le(rhcu16 x) {
    rhcu16 result;
    rhcu8 *c;
    c = (rhcu8 *) &result;
    c[1] = (x >> 8) & 0xFF;
    c[0] = x & 0xFF;
    return result;
}

static rhcu16 rhc_endian_u16_le_to_host(rhcu16 x) {
    rhcu16 result;
    rhcu8 *c;

    c = (rhcu8 *) &x;
    result = (rhcu16) c[0];
    result |= (rhcu16) c[1] << 8;
    return result;
}

static rhcu16 rhc_endian_u16_be_to_host(rhcu16 x) {
    rhcu16 result;
    rhcu8 *c;

    c = (rhcu8 *) &x;
    result = (rhcu16) c[1];
    result |= (rhcu16) c[0] << 8;
    return result;
}

static rhcu32 rhc_endian_u32_host_to_be(rhcu32 x) {
    rhcu32 result;
    rhcu8 *c;
    c = (rhcu8 *) &result;
    c[0] = (x >> 24) & 0xFF;
    c[1] = (x >> 16) & 0xFF;
    c[2] = (x >> 8) & 0xFF;
    c[3] = x & 0xFF;
    return result;
}

static rhcu32 rhc_endian_u32_host_to_le(rhcu32 x) {
    rhcu32 result;
    rhcu8 *c;
    c = (rhcu8 *) &result;
    c[3] = (x >> 24) & 0xFF;
    c[2] = (x >> 16) & 0xFF;
    c[1] = (x >> 8) & 0xFF;
    c[0] = x & 0xFF;
    return result;
}

static rhcu32 rhc_endian_u32_le_to_host(rhcu32 x) {
    rhcu32 result;
    rhcu8 *c;

    c = (rhcu8 *) &x;
    result = (rhcu32) c[0];
    result |= (rhcu32) c[1] << 8;
    result |= (rhcu32) c[2] << 16;
    result |= (rhcu32) c[3] << 24;
    return result;
}

static rhcu32 rhc_endian_u32_be_to_host(rhcu32 x) {
    rhcu32 result;
    rhcu8 *c;

    c = (rhcu8 *) &x;
    result = (rhcu32) c[3];
    result |= (rhcu32) c[2] << 8;
    result |= (rhcu32) c[1] << 16;
    result |= (rhcu32) c[0] << 24;
    return result;
}

static rhcu64 rhc_endian_u64_host_to_be(rhcu64 x) {
    rhcu64 result;
    rhcu8 *c;
    c = (rhcu8 *) &result;
    c[0] = x >> 56;
    c[1] = (x >> 48) & 0xFF;
    c[2] = (x >> 40) & 0xFF;
    c[3] = (x >> 32) & 0xFF;
    c[4] = (x >> 24) & 0xFF;
    c[5] = (x >> 16) & 0xFF;
    c[6] = (x >> 8) & 0xFF;
    c[7] = x & 0xFF;
    return result;
}

static rhcu64 rhc_endian_u64_host_to_le(rhcu64 x) {
    rhcu64 result;
    rhcu8 *c;
    c = (rhcu8 *) &result;
    c[7] = x >> 56;
    c[6] = (x >> 48) & 0xFF;
    c[5] = (x >> 40) & 0xFF;
    c[4] = (x >> 32) & 0xFF;
    c[3] = (x >> 24) & 0xFF;
    c[2] = (x >> 16) & 0xFF;
    c[1] = (x >> 8) & 0xFF;
    c[0] = x & 0xFF;
    return result;
}

static rhcu64 rhc_endian_u64_le_to_host(rhcu64 x) {
    rhcu64 result;
    rhcu8 *c;

    c = (rhcu8 *) &x;
    result = (rhcu64) c[0];
    result |= (rhcu64) c[1] << 8;
    result |= (rhcu64) c[2] << 16;
    result |= (rhcu64) c[3] << 24;
    result |= (rhcu64) c[4] << 32;
    result |= (rhcu64) c[5] << 40;
    result |= (rhcu64) c[6] << 48;
    result |= (rhcu64) c[7] << 56;
    return result;
}

static rhcu64 rhc_endian_u64_be_to_host(rhcu64 x) {
    rhcu64 result;
    rhcu8 *c;

    c = (rhcu8 *) &x;
    result = (rhcu64) c[7];
    result |= (rhcu64) c[6] << 8;
    result |= (rhcu64) c[5] << 16;
    result |= (rhcu64) c[4] << 24;
    result |= (rhcu64) c[3] << 32;
    result |= (rhcu64) c[2] << 40;
    result |= (rhcu64) c[1] << 48;
    result |= (rhcu64) c[0] << 56;
    return result;
}


//
// signed wrapper
//

static rhci16 rhc_endian_i16_host_to_be(rhci16 x) {
    rhcu16 ret = rhc_endian_u16_host_to_be(*((rhcu16*)&x));
    return *((rhci16*)&ret);
}

static rhci16 rhc_endian_i16_host_to_le(rhci16 x) {
    rhcu16 ret = rhc_endian_u16_host_to_le(*((rhcu16*)&x));
    return *((rhci16*)&ret);
}

static rhci16 rhc_endian_i16_le_to_host(rhci16 x) {
    rhcu16 ret = rhc_endian_u16_le_to_host(*((rhcu16*)&x));
    return *((rhci16*)&ret);
}

static rhci16 rhc_endian_i16_be_to_host(rhci16 x) {
    rhcu16 ret = rhc_endian_u16_be_to_host(*((rhcu16*)&x));
    return *((rhci16*)&ret);
}


static rhci32 rhc_endian_i32_host_to_be(rhci32 x) {
    rhcu32 ret = rhc_endian_u32_host_to_be(*((rhcu32*)&x));
    return *((rhci32*)&ret);
}

static rhci32 rhc_endian_i32_host_to_le(rhci32 x) {
    rhcu32 ret = rhc_endian_u32_host_to_le(*((rhcu32*)&x));
    return *((rhci32*)&ret);
}

static rhci32 rhc_endian_i32_le_to_host(rhci32 x) {
    rhcu32 ret = rhc_endian_u32_le_to_host(*((rhcu32*)&x));
    return *((rhci32*)&ret);
}

static rhci32 rhc_endian_i32_be_to_host(rhci32 x) {
    rhcu32 ret = rhc_endian_u32_be_to_host(*((rhcu32*)&x));
    return *((rhci32*)&ret);
}


static rhci64 rhc_endian_i64_host_to_be(rhci64 x) {
    rhcu64 ret = rhc_endian_u64_host_to_be(*((rhcu64*)&x));
    return *((rhci64*)&ret);
}

static rhci64 rhc_endian_i64_host_to_le(rhci64 x) {
    rhcu64 ret = rhc_endian_u64_host_to_le(*((rhcu64*)&x));
    return *((rhci64*)&ret);
}

static rhci64 rhc_endian_i64_le_to_host(rhci64 x) {
    rhcu64 ret = rhc_endian_u64_le_to_host(*((rhcu64*)&x));
    return *((rhci64*)&ret);
}

static rhci64 rhc_endian_i64_be_to_host(rhci64 x) {
    rhcu64 ret = rhc_endian_u64_be_to_host(*((rhcu64*)&x));
    return *((rhci64*)&ret);
}

#endif //RHC_ENDIAN_H