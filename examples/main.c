
//#define RHC_LOG_DO_NOT_PRINT_TIME_FILE
//#define RHC_LOG_DO_NOT_USE_COLOR
#include "rhc/rhc_impl.h"
#include "rhc/rhc_full.h"


int main() {
    rhc_assume(0, "wtf");
    rhc_log_trace("hey");
    rhc_log_debug("hey");
    rhc_log_info("hey");
    rhc_log_warn("hey");
    rhc_log_error("hey");
    rhc_log_wtf("hey");

    RhcFile *f = rhc_file_new_write("test.txt", true);
    RhcStream_i s = rhc_file_stream(f);
    char msg[] = "Hello World\n";
    rhc_stream_write(s, msg, sizeof msg - 1);
    rhc_file_kill(&f);
}

