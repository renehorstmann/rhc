#ifndef RHC_STREAM_H
#define RHC_STREAM_H

//
// Stream interface and functions
//    Implementations are in string.h, file.h and socket.h
//

#include "export.h"
#include "common.h"

struct RhcString;

//
// Interface
//

struct RhcStream_i;

typedef rhcsize (*rhc_stream_read_try_fn)(struct RhcStream_i *stream, void *memory, rhcsize n);
typedef rhcsize (*rhc_stream_write_try_fn)(struct RhcStream_i *stream, const void *memory, rhcsize n);
typedef void (*rhc_stream_flush_fn)(struct RhcStream_i *stream);
typedef bool (*rhc_stream_valid_fn)(struct RhcStream_i *stream);

typedef struct RhcStream_i {
    void *impl;

    // vtable
    rhc_stream_read_try_fn opt_read_try;
    rhc_stream_write_try_fn opt_write_try;
    rhc_stream_flush_fn opt_flush;
    rhc_stream_valid_fn valid;
} RhcStream_i;


//
// Functions
//

static RhcStream_i rhc_stream_new_invalid() {
    return (RhcStream_i) {0};
}

// trys to reads up to n bytes from the stream into memory
// returns bytes read or <=0 on error
static rhcsize rhc_stream_read_try(RhcStream_i self, void *memory, rhcsize n) {
    assert(self.opt_read_try);
    return self.opt_read_try(&self, memory, n);
}
    
// trys writes up to n bytes into the stream from memory
// returns bytes written or <=0 on error
static rhcsize rhc_stream_write_try(RhcStream_i self, const void *memory, rhcsize n) {
    assert(self.opt_write_try);
    return self.opt_write_try(&self, memory, n);
}

// flushes the stream, if opt_flush is available
static void rhc_stream_flush(RhcStream_i self) {
    if(self.opt_flush)
        self.opt_flush(&self);
}

// returns true, if the stream (implementation) is in a valid state
static bool rhc_stream_valid(RhcStream_i self) {
    if(!self.valid)
        return false;
    return self.valid(&self);
}

// reads exactly n bytes from the stream into memory
// returns bytes read or <=0 on error
RHC_EXPORT
rhcsize rhc_stream_read(RhcStream_i self, void *memory, rhcsize n);

// reads exactly n bytes, if the stream stays valid
// returns bytes read or <=0 on error
RHC_EXPORT
rhcsize rhc_stream_read_as_long_as_valid(RhcStream_i self, void *memory, rhcsize n);

// writes exactly n bytes into the stream into memory
// returns bytes written or <=0 on error
RHC_EXPORT
rhcsize rhc_stream_write(RhcStream_i self, const void *memory, rhcsize n);

// print formatted on the stream
RHC_EXPORT
rhcsize rhc_stream_printf_v(RhcStream_i self, const char *format, va_list args);

// print formatted on the stream
RHC_EXPORT
rhcsize rhc_stream_printf(RhcStream_i self, const char *format, ...);

// print a line of text on the stream
RHC_EXPORT
rhcsize rhc_stream_puts(RhcStream_i self, const char *text);

// reads a single byte from the stream
// returns -1 on error
static int rhc_stream_getchar(RhcStream_i self) {
    char c;
    if(rhc_stream_read_try(self, &c, 1) <= 0)
        return -1;
    return (int) c;
}

// writes a single byte into the stream
// returns false on error
static bool rhc_stream_putchar(RhcStream_i self, char c) {
    return rhc_stream_write_try(self, &c, 1) == 1;
}

// reads from the string into memory, until the given char "until" (which will be included)
// stops on max characters. To check if there are more characters before "until", simple check:
//      if(returned_line_len==max && memory[returned_line_len-1]!=until)
//          // more characters in the line
// returns 0 on error
RHC_EXPORT
rhcsize rhc_stream_read_until_into(RhcStream_i self, void *memory, rhcsize max, char until);

// reads into the returned String, until the given char "until" (which will be included)
// returns an invalid string on error
RHC_EXPORT
struct RhcString *rhc_stream_read_until(RhcStream_i self, char until);


// reads from the string into memory, until newline (which will be included)
// stops on max characters. To check if there are more characters in the line, simple check:
//      if(returned_line_len==max && memory[returned_line_len-1]!='\n')
//          // more characters in the line
// returns 0 on error
static rhcsize rhc_stream_read_line_into(RhcStream_i self, void *memory, rhcsize max) {
    return rhc_stream_read_until_into(self, memory, max, '\n');
}

// reads into the returned String, until newline (which will be included)
// returns an invalid string on error
static struct RhcString *rhc_stream_read_line(RhcStream_i self) {
    return rhc_stream_read_until(self, '\n');
}



#endif //RHC_STREAM_H
