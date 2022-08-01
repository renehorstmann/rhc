#ifndef RHC_STR_IMPL_H
#define RHC_STR_IMPL_H
#ifdef RHC_IMPL

#include "../file.h"
#include "../str.h"

rhcsize rhc_str_stream_print(RhcStr_s str, RhcStream_i stream) {
    if(!rhc_str_valid(str))
        return rhc_stream_printf(stream, "(RhcStr_s) {NULL, 0}\n");
    return rhc_stream_printf(stream, "(RhcStr_s) {\"%.*s\", %zu}\n", (int) str.size, str.data, str.size);
}

rhcsize rhc_str_print(RhcStr_s str) {
    return rhc_str_stream_print(str, rhc_file_stream_stdout());
}

#endif //RHC_IMPL
#endif //RHC_STR_IMPL_H
