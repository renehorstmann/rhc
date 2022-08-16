#ifndef RHC_FILE_IMPL_H
#define RHC_FILE_IMPL_H
#ifdef RHC_IMPL

#include "../error.h"
#include "../file.h"

#ifdef OPTION_SDL
#include <SDL2/SDL.h>
#endif

struct RhcFile {
#ifdef OPTION_SDL
    SDL_RWops *file;
#else
    FILE *file;
#endif
};


bool rhc_file_valid(RhcFile *self) {
    return self && self->file;
}

static void rhc__file_close(RhcFile *self) {
    if (!self || !self->file)
        return;
#ifdef OPTION_SDL
    SDL_RWclose(self->file);
#else
    fclose(self->file);
#endif
    self->file = NULL;
}


RhcFile *rhc_file_new_read(const char *file, bool ascii) {
    RhcFile self_stack;
#ifdef OPTION_SDL
    self_stack.file = SDL_RWFromFile(file, ascii ? "r" : "rb");
#else
    self_stack.file = fopen(file, ascii ? "r" : "rb");
#endif
    if (!rhc_file_valid(&self_stack)) {
        rhc_error_set("rhc_file_new_read failed");
        rhc_log_warn("rhc_file_new_read failed: %s", file);
        return NULL;
    }
    RhcFile *self = rhc_new(RhcFile, 1);
    *self = self_stack;
    return self;
}

RhcFile *rhc_file_new_write(const char *file, bool ascii) {
    RhcFile self_stack;
#ifdef OPTION_SDL
    self_stack.file = SDL_RWFromFile(file, ascii ? "w" : "wb");
#else
    self_stack.file = fopen(file, ascii ? "w" : "wb");
#endif
    if (!rhc_file_valid(&self_stack)) {
        rhc_error_set("rhc_file_new_write failed");
        rhc_log_warn("rhc_file_new_write failed: %s", file);
        return NULL;
    }
    RhcFile *self = rhc_new(RhcFile, 1);
    *self = self_stack;
    return self;
}

RhcFile *rhc_file_new_append(const char *file, bool ascii) {
    RhcFile self_stack;
#ifdef OPTION_SDL
    self_stack.file = SDL_RWFromFile(file, ascii ? "a" : "ab");
#else
    self_stack.file = fopen(file, ascii ? "a" : "ab");
#endif
    if (!rhc_file_valid(&self_stack)) {
        rhc_error_set("rhc_file_new_append failed");
        rhc_log_warn("rhc_file_new_append failed: %s", file);
        return NULL;
    }
    RhcFile *self = rhc_new(RhcFile, 1);
    *self = self_stack;
    return self;
}

void rhc_file_kill(RhcFile **self_ptr) {
    RhcFile *self = *self_ptr;
    rhc__file_close(self);
    rhc_free(self);
    *self_ptr = NULL;
}

rhcsize rhc_file_size(RhcFile *self) {
    if(!rhc_file_valid(self))
        return 0;
#ifdef OPTION_SDL
    Sint64 rwsize = SDL_RWsize(self->file);
    if(rwsize <=0) {
        rhc_error_set("file read failed");
        rhc_log_warn("rhc_file_size failed: (unknown size)");
    }

    return (rhcsize) rhc_max(rwsize, 0);
#else
    fseek(self->file, 0, SEEK_END);
    rhcsize length = (rhcsize) ftell(self->file);
    fseek(self->file, 0, SEEK_SET);
    return rhc_max(length, 0);
#endif
}

static rhcsize rhc__file_stream_read(struct RhcStream_i *stream, void *memory, rhcsize n) {
    RhcFile *self = stream->impl;
    if (!rhc_file_valid(self)) {
        return 0;
    }
    rhcsize read;
#ifdef OPTION_SDL
    read = (rhcsize) SDL_RWread(self->file, memory, 1, n);
#else
    read = (rhcsize) fread(memory, 1, n, self->file);
#endif
    if (read <= 0) {
        rhc__file_close(self);
    }
    return read;
}

static rhcsize rhc__file_stream_write(struct RhcStream_i *stream, const void *memory, rhcsize n) {
    RhcFile *self = stream->impl;
    if (!rhc_file_valid(self)) {
        return 0;
    }
    rhcsize written;
#ifdef OPTION_SDL
    written = (rhcsize) SDL_RWwrite(self->file, memory, 1, n);
#else
    written = (rhcsize) fwrite(memory, 1, n, self->file);
#endif
    if (written <= 0) {
        rhc__file_close(self);
    }
    return written;
}

static void rhc__file_stream_flush(struct RhcStream_i *stream) {
    RhcFile *self = stream->impl;
    if (!rhc_file_valid(self)) {
        return;
    }
    // sdl not seems to have a flush function
#ifndef OPTION_SDL
    fflush(self->file);
#endif
}

static bool rhc__file_stream_valid(struct RhcStream_i *stream) {
    RhcFile *self = stream->impl;
    return rhc_file_valid(self);
}


RhcStream_i rhc_file_stream(RhcFile *self) {
    return (RhcStream_i) {
            .impl = self,
            .opt_read_try = rhc__file_stream_read,
            .opt_write_try = rhc__file_stream_write,
            .opt_flush = rhc__file_stream_flush,
            .valid = rhc__file_stream_valid};
}


RhcStream_i rhc_file_stream_from_cfile(FILE *cfile, bool read, bool write) {
    static RhcFile f = {0};
#ifdef OPTION_SDL
    f.file = SDL_RWFromFP(cfile, SDL_FALSE);
#else
    f.file = cfile;
#endif
    RhcStream_i stream = rhc_file_stream_out(&f);
    if(!read)
        stream.opt_read_try = NULL;
    if(!write)
        stream.opt_write_try = NULL;
    return stream;
}

RhcString *rhc_file_read_a(const char *file, bool ascii, RhcAllocator_i a) {
    if (!rhc_allocator_valid(a)) {
        rhc_error_set("file read failed, allocator invalid");
        rhc_log_error("rhc_file_read_a failed, allocator invalid: %s", file);
        return rhc_string_new_invalid();
    }
    RhcFile *f = rhc_file_new_read(file, ascii);

    // will return an empty (allocated) string else
    if(!rhc_file_valid(f))
        return rhc_string_new_invalid();

    rhcsize length = rhc_file_size(f);
    RhcString *res = rhc_string_new_a(length, a);
    res->size = rhc_stream_read_as_long_as_valid(rhc_file_stream_in(f), res->data, length);
    rhc_file_kill(&f);
    return res;
}

bool rhc_file_write(const char *file, RhcStr_s content, bool ascii) {
    RhcFile *f = rhc_file_new_write(file, ascii);
    rhcsize s = rhc_stream_write(rhc_file_stream_out(f), content.data, content.size);
    rhc_file_kill(&f);
    return s > 0;
}

bool rhc_file_append(const char *file, RhcStr_s content, bool ascii) {
    RhcFile *f = rhc_file_new_append(file, ascii);
    rhcsize s = rhc_stream_write(rhc_file_stream_out(f), content.data, content.size);
    rhc_file_kill(&f);
    return s > 0;
}

#endif //RHC_IMPL
#endif //RHC_FILE_IMPL_H
