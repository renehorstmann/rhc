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

static rhcu16 rhc_endian_host_to_be16(rhcu16 x) {
    rhcu16 result;
    unsigned char *c;
    c = (unsigned char *) &result;
    c[0] = (x >> 8) & 0xFF;
    c[1] = x & 0xFF;
    return result;
}

static rhcu16 rhc_endian_host_to_le16(rhcu16 x) {
    rhcu16 result;
    unsigned char *c;
    c = (unsigned char *) &result;
    c[1] = (x >> 8) & 0xFF;
    c[0] = x & 0xFF;
    return result;
}

static rhcu16 rhc_endian_le_to_host16(rhcu16 x) {
    rhcu16 result;
    unsigned char *c;

    c = (unsigned char *) &x;
    result = (rhcu16) c[0];
    result |= (rhcu16) c[1] << 8;
    return result;
}

static rhcu16 rhc_endian_be_to_host16(rhcu16 x) {
    rhcu16 result;
    unsigned char *c;

    c = (unsigned char *) &x;
    result = (rhcu16) c[1];
    result |= (rhcu16) c[0] << 8;
    return result;
}

static rhcu32 rhc_endian_host_to_be32(rhcu32 x) {
    rhcu32 result;
    unsigned char *c;
    c = (unsigned char *) &result;
    c[0] = (x >> 24) & 0xFF;
    c[1] = (x >> 16) & 0xFF;
    c[2] = (x >> 8) & 0xFF;
    c[3] = x & 0xFF;
    return result;
}

static rhcu32 rhc_endian_host_to_le32(rhcu32 x) {
    rhcu32 result;
    unsigned char *c;
    c = (unsigned char *) &result;
    c[3] = (x >> 24) & 0xFF;
    c[2] = (x >> 16) & 0xFF;
    c[1] = (x >> 8) & 0xFF;
    c[0] = x & 0xFF;
    return result;
}

static rhcu32 rhc_endian_le_to_host32(rhcu32 x) {
    rhcu32 result;
    unsigned char *c;

    c = (unsigned char *) &x;
    result = (rhcu32) c[0];
    result |= (rhcu32) c[1] << 8;
    result |= (rhcu32) c[2] << 16;
    result |= (rhcu32) c[3] << 24;
    return result;
}

static rhcu32 rhc_endian_be_to_host32(rhcu32 x) {
    rhcu32 result;
    unsigned char *c;

    c = (unsigned char *) &x;
    result = (rhcu32) c[3];
    result |= (rhcu32) c[2] << 8;
    result |= (rhcu32) c[1] << 16;
    result |= (rhcu32) c[0] << 24;
    return result;
}

static rhcu64 rhc_endian_host_to_be64(rhcu64 x) {
    rhcu64 result;
    unsigned char *c;
    c = (unsigned char *) &result;
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

static rhcu64 rhc_endian_host_to_le64(rhcu64 x) {
    rhcu64 result;
    unsigned char *c;
    c = (unsigned char *) &result;
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

static rhcu64 rhc_endian_le_to_host64(rhcu64 x) {
    rhcu64 result;
    unsigned char *c;

    c = (unsigned char *) &x;
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

static rhcu64 rhc_endian_be_to_host64(rhcu64 x) {
    rhcu64 result;
    unsigned char *c;

    c = (unsigned char *) &x;
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

#endif //RHC_ENDIAN_H
