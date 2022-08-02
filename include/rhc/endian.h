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

static void rhc_endian_host_to_le16(void *le16, const void *host16) {
    rhcu16 h = *(const rhcu16*) host16;
    rhcu8 *x = (rhcu8 *) le16;
    x[1] = (h >> 8) & 0xFF;
    x[0] = h & 0xFF;
}

static void rhc_endian_host_to_be16(void *be16, const void *host16) {
    rhcu16 h = *(const rhcu16*) host16;
    rhcu8 *x = (rhcu8 *) be16;
    x[0] = (h >> 8) & 0xFF;
    x[1] = h & 0xFF;
}

static void rhc_endian_le_to_host16(void *host16, const void *le16) {
    rhcu16 h = *(rhcu16*) host16;
    const rhcu8 *x = (const rhcu8 *) le16;
    h = (rhcu16) x[0];
    h |= (rhcu16) x[1] << 8;
}

static void rhc_endian_be_to_host16(void *host16, const void *be16) {
    rhcu16 h = *(rhcu16*) host16;
    const rhcu8 *x = (const rhcu8 *) be16;
    h = (rhcu16) x[1];
    h |= (rhcu16) x[0] << 8;
}



static void rhc_endian_host_to_le32(void *le32, const void *host32) {
    rhcu32 h = *(const rhcu32*) host32;
    rhcu8 *x = (rhcu8 *) le32;
    x[3] = (h >> 24) & 0xFF;
    x[2] = (h >> 16) & 0xFF;
    x[1] = (h >> 8) & 0xFF;
    x[0] = h & 0xFF;
}

static void rhc_endian_host_to_be32(void *be32, const void *host32) {
    rhcu32 h = *(const rhcu32*) host32;
    rhcu8 *x = (rhcu8 *) be32;
    x[0] = (h >> 24) & 0xFF;
    x[1] = (h >> 16) & 0xFF;
    x[2] = (h >> 8) & 0xFF;
    x[3] = h & 0xFF;
}

static void rhc_endian_le_to_host32(void *host32, const void *le32) {
    rhcu32 h = *(rhcu32*) host32;
    const rhcu8 *x = (const rhcu8 *) le32;
    h = (rhcu32) x[0];
    h |= (rhcu32) x[1] << 8;
    h |= (rhcu32) x[2] << 16;
    h |= (rhcu32) x[3] << 24;
}

static void rhc_endian_be_to_host32(void *host32, const void *be32) {
    rhcu32 h = *(rhcu32*) host32;
    const rhcu8 *x = (const rhcu8 *) be32;
    h = (rhcu32) x[3];
    h |= (rhcu32) x[2] << 8;
    h |= (rhcu32) x[1] << 16;
    h |= (rhcu32) x[0] << 24;
}



static void rhc_endian_host_to_le64(void *le64, const void *host64) {
    rhcu64 h = *(const rhcu64*) host64;
    rhcu8 *x = (rhcu8 *) le64;
    x[7] = (h >> 56) & 0xFF;
    x[6] = (h >> 48) & 0xFF;
    x[5] = (h >> 40) & 0xFF;
    x[4] = (h >> 32) & 0xFF;
    x[3] = (h >> 24) & 0xFF;
    x[2] = (h >> 16) & 0xFF;
    x[1] = (h >> 8) & 0xFF;
    x[0] = h & 0xFF;
}

static void rhc_endian_host_to_be64(void *be64, const void *host64) {
    rhcu64 h = *(const rhcu64*) host64;
    rhcu8 *x = (rhcu8 *) be64;
    x[0] = (h >> 56) & 0xFF;
    x[1] = (h >> 48) & 0xFF;
    x[2] = (h >> 40) & 0xFF;
    x[3] = (h >> 32) & 0xFF;
    x[4] = (h >> 24) & 0xFF;
    x[5] = (h >> 16) & 0xFF;
    x[6] = (h >> 8) & 0xFF;
    x[7] = h & 0xFF;
}

static void rhc_endian_le_to_host64(void *host64, const void *le64) {
    rhcu64 h = *(rhcu64*) host64;
    const rhcu8 *x = (const rhcu8 *) le64;
    h = (rhcu64) x[0];
    h |= (rhcu64) x[1] << 8;
    h |= (rhcu64) x[2] << 16;
    h |= (rhcu64) x[3] << 24;
    h |= (rhcu64) x[4] << 32;
    h |= (rhcu64) x[5] << 40;
    h |= (rhcu64) x[6] << 48;
    h |= (rhcu64) x[7] << 56;
}

static void rhc_endian_be_to_host64(void *host64, const void *be64) {
    rhcu64 h = *(rhcu64*) host64;
    const rhcu8 *x = (const rhcu8 *) be64;
    h = (rhcu64) x[7];
    h |= (rhcu64) x[6] << 8;
    h |= (rhcu64) x[5] << 16;
    h |= (rhcu64) x[4] << 24;
    h |= (rhcu64) x[3] << 32;
    h |= (rhcu64) x[2] << 40;
    h |= (rhcu64) x[1] << 48;
    h |= (rhcu64) x[0] << 56;
}


#endif //RHC_ENDIAN_H
