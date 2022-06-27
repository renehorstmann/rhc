#ifndef RHC_STREAM_IMPL_H
#define RHC_STREAM_IMPL_H
#ifdef RHC_IMPL

#include "../memory.h"
#include "../string.h"
#include "../stream.h"

#define RHC_STREAM_STATIC_BUF_SIZE 128

rhcsize rhc_stream_read(RhcStream_i self, void *memory, rhcsize n) {
    rhcsize read = 0;
    do {
        rhcsize r = rhc_stream_read_try(self, (rhcu8 *) memory + read, n - read);
        if(r<=0)
            return 0;
        read+=r;
    } while (read < n);
    return read;
}

rhcsize rhc_stream_read_as_long_as_valid(RhcStream_i self, void *memory, rhcsize n) {
    rhcsize read = 0;
    do {
        rhcsize r = rhc_stream_read_try(self, (rhcu8 *) memory + read, n - read);
        if(r<=0)
            return read;
        read+=r;
    } while (read < n);
    return read;
}

rhcsize rhc_stream_write(RhcStream_i self, const void *memory, rhcsize n) {
    rhcsize written = 0;
    do {
        rhcsize r = rhc_stream_write_try(self, (rhcu8 *) memory + written, n - written);
        if(r<=0)
            return 0;
        written+=r;
    } while (written < n);
    return written;
}

rhcsize rhc_stream_printf_v(RhcStream_i self, const char *format, va_list args) {
    char static_buf[RHC_STREAM_STATIC_BUF_SIZE];

    va_list cpy;
    va_copy(cpy, args);
    rhcsize s = vsnprintf(NULL, 0, format, args);
    char *buf;
    if(s+1<=RHC_STREAM_STATIC_BUF_SIZE)
        buf = static_buf;
    else
        buf = rhc_malloc(s+1);

    rhcsize s2 = vsnprintf(buf, s + 1, format, cpy);
    assert(s==s2 && "different sizes from vsprintf?");
    rhcsize ret = rhc_stream_write(self, buf, s);

    if(buf != static_buf)
        rhc_free(buf);
    va_end(cpy);
    return ret;
}

rhcsize rhc_stream_printf(RhcStream_i self, const char *format, ...) {
    va_list args;
    va_start(args, format);
    rhcsize ret = rhc_stream_printf_v(self, format, args);
    va_end(args);
    return ret;
}

rhcsize rhc_stream_puts(RhcStream_i self, const char *text) {
    rhcsize len = (rhcsize) strlen(text);
    len = rhc_stream_write(self, text, len);
    len += rhc_stream_write(self, "\n", 1);
    rhc_stream_flush(self);
    return len;
}

rhcsize rhc_stream_read_until_into(RhcStream_i self, void *memory, rhcsize max, char until) {
    assert(max>0 && "minimum 1 byte for the line");
    rhcsize len = 0;
    do {
        int r = rhc_stream_getchar(self);
        if(r<0)
            return 0;
        ((rhcu8*) memory)[len++] = r;
        if(r==until)
            break;
    } while(len<max);
    return len;
}

struct RhcString *rhc_stream_read_until(RhcStream_i self, char until) {
    RhcString *s = rhc_string_new(32);
    RhcStream_i ss = rhc_string_get_stream(s);
    for(;;) {
        int r = rhc_stream_getchar(self);
        if(r<0) {
            rhc_string_kill(&s);
            return rhc_string_new_invalid();
        }
        rhc_stream_putchar(ss, (char) r);
        if(r==until)
            break;
    }
    return s;
}


#endif //RHC_IMPL
#endif //RHC_STREAM_IMPL_H
