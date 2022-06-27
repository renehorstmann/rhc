#ifndef RHC_FILE_H
#define RHC_FILE_H

//
// open, read and write files
// get a RhcStream_i from the file
//

#include "string.h"

#define RHC_FILE_STORAGE_SIZE 8

typedef struct RhcFile RhcFile;


bool rhc_file_valid(RhcFile *self);

// opens a file to read
RhcFile *rhc_file_new_read(const char *file, bool ascii);

// opens a file to write
RhcFile *rhc_file_new_write(const char *file, bool ascii);

// opens a file to append
RhcFile *rhc_file_new_append(const char *file, bool ascii);

// closes the file
void rhc_file_kill(RhcFile **self_ptr);

// returns the size of the whole file, returns 0 on error
rhcsize rhc_file_size(RhcFile *self);

// returns a stream from the file (read and write)
RhcStream_i rhc_file_stream(RhcFile *file);

// returns an input/read stream from the file
static RhcStream_i rhc_file_stream_in(RhcFile *self) {
    RhcStream_i stream = rhc_file_stream(self);
    stream.opt_write_try = NULL;
    return stream;
}

// returns an output/write stream from the file
static RhcStream_i rhc_file_stream_out(RhcFile *self) {
    RhcStream_i stream = rhc_file_stream(self);
    stream.opt_read_try = NULL;
    return stream;
}

// creates a stream from a c FILE*
RhcStream_i rhc_file_stream_from_cfile(FILE *cfile, bool read, bool write);

// Returns a stream from the system file
static RhcStream_i rhc_file_stream_stdout() {
    return rhc_file_stream_from_cfile(stdout, false, true);
}

// Returns a stream from the system file
static RhcStream_i rhc_file_stream_stderr() {
    return rhc_file_stream_from_cfile(stderr, false, true);
}

// Returns a stream from the system file
static RhcStream_i rhc_file_stream_stdin() {
    return rhc_file_stream_from_cfile(stdin, true, false);
}


// reads in a full file into the returned RhcString
RhcString *rhc_file_read_a(const char *file, bool ascii, RhcAllocator_i a);

static RhcString *rhc_file_read(const char *file, bool ascii) {
    return rhc_file_read_a(file, ascii, RHC_ALLOCATOR_DEFAULT);
}

// writes a file and returns false on error
bool rhc_file_write(const char *file, RhcStr_s content, bool ascii);

// appends to a file and returns false on error
bool rhc_file_append(const char *file, RhcStr_s content, bool ascii);

#endif //RHC_FILE_H
