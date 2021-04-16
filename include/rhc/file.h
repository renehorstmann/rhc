#ifndef RHC_FILE_H
#define RHC_FILE_H

#include "types.h"
#include "alloc.h"

enum file_read_mode {
    FILE_READ_MODE_ASCII,
    FILE_READ_MODE_BINARY,
    FILE_READ_NUM_MODES,
};

enum file_write_mode {
    FILE_WRITE_MODE_ASCII,
    FILE_WRITE_MODE_BINARY,
    FILE_WRITE_MODE_ASCII_APPEND,
    FILE_WRITE_MODE_BINARY_APPEND,
    FILE_WRITE_NUM_MODES,
};


String file_read_a(const char *file, enum file_read_mode mode, Allocator_s a);

static String file_read(const char *file, enum file_read_mode mode) {
    return file_read_a(file, mode, RHC_STRING_DEFAULT_ALLOCATOR);
}

bool file_write(const char *file, str content, enum file_write_mode mode);

#endif //RHC_FILE_H
