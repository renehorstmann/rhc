#ifndef RHC_FILE_IMPL_H
#define RHC_FILE_IMPL_H
#ifdef RHC_IMPL

#include <stdio.h>
#include "../error.h"
#include "../str.h"
#include "../string.h"
#include "../log.h"
#include "../file.h"

#ifdef OPTION_SDL
#include <SDL.h>
#endif


#ifdef OPTION_SDL
String file_read_a(const char *file, enum file_read_mode mode, Allocator_s a) {
    SDL_RWops *f = SDL_RWFromFile(file, mode == FILE_READ_MODE_ASCII ? "r" : "rb");
    if (!f) {
        rhc_error = "file read failed";
        log_warn("file read failed: %s", file);
        return String_new_invalid_a(a);
    }

    Sint64 length = SDL_RWsize(rw);
    String res = String_new_a(length, a);

    if (String_valid(red)) {
        Sint64 chars_read = 0, buf_appended = 1;
        char *buf = res.s.data;
        while (chars_read < res_size && buf_appended != 0) {
            buf_appended = SDL_RWread(f, buf, 1, (res_size - chars_read));
            chars_read += buf_appended;
            buf += buf_appended;
        }
        if (chars_read != length) {
            log_error("reading file failed: %s %d/%d bytes read", file, chars_read, length);
            String_delete(&res);
        } else {
            res.s.data[length] = '\0';  // should have been set in String, just to be sure
        }
    }
    SDL_RWclose(f);
    return res;
}
#else

String file_read_a(const char *file, enum file_read_mode mode, Allocator_s a) {
    FILE *f = fopen(file, mode == FILE_READ_MODE_ASCII ? "r" : "rb");
    if (!f) {
        rhc_error = "file read failed";
        log_warn("file read failed: %s", file);
        return String_new_invalid_a(a);
    }

    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    String res = String_new_a(length, a);

    if (String_valid(res)) {
        size_t chars_read = fread(res.s.data, 1, length, f);
        if (chars_read != length) {
            log_error("reading file failed: %s %d/%d bytes read", file, chars_read, length);
            String_delete(&res);
        } else {
            res.s.data[length] = '\0';  // should have been set in String, just to be sure
        }
    }
    fclose(f);
    return res;
}

#endif

#endif //RHC_IMPL
#endif //RHC_FILE_IMPL_H
