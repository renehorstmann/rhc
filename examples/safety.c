// only in a single header file, to implement some rhc source functions
//    use rhc/rhc.h or single headers instead
#include "rhc/rhc_impl.h"

int main() {

    // should end in an invalid String
    String file = file_read("not_available.txt", true);
    // produces a log_warn: file read failed: not_available.txt

    // should not break
    str_tolower(file.str);

    // should return 0
    int cnt = str_count(file.str, 'a');

    // should not write and set ok to false
    bool ok = file_write("not_available.txt", file.str, true);
    // producus a log_error: file write failed, content invalid: not_available.txt
    // sets rhc_error = file write failed, content invalid

    printf("cnt: %i, file_valid: %i, ok: %i, error_msg: %s\n", cnt, string_valid(file), ok, rhc_error);
    // cnt: 0, file_valid: 0, ok: 0, error_msg: file write failed, content invalid
}
