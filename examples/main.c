
//#define RHC_LOG_DO_NOT_PRINT_TIME_FILE
//#define RHC_LOG_DO_NOT_USE_COLOR
#include "rhc/rhc_impl.h"


int main() {
    log_trace("hey");
    log_debug("hey");
    log_info("hey");
    log_warn("hey");
    log_error("hey");
    log_wtf("hey");

    RhcFile *f = rhc_file_open_write("test.txt", true);
    Stream_i s = rhc_file_get_stream(f);
    char msg[] = "Hello World\n";
    stream_write_msg(s, msg, sizeof msg - 1);
    rhc_file_kill(&f);
}

