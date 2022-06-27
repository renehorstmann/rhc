// only in a single header file, to implement some rhc source functions
//    use rhc/rhc.h or single headers instead
#include "rhc/rhc_impl.h"

int main() {

    // should end in an invalid RhcString
    RhcString *file = rhc_file_read("not_available.txt", true);
    // produces a log_warn: file read failed: not_available.txt

    // returns an invalid str, if file is not valid
    RhcStr_s str = rhc_string_get_str(file);

    // should not break
    rhc_str_tolower(str);

    // should return 0
    rhcsize cnt = rhc_str_count(str, 'a');

    // should not write and set ok to false
    bool ok = rhc_file_write("also_not_available.txt", str, true);
    // producus a log_error: file write failed, content invalid: not_available.txt
    // sets rhc_error = file write failed, content invalid

    printf("cnt: %i, rhc_file_valid: %i, ok: %i, error_msg: %s\n",
           (int) cnt, rhc_string_valid(file), ok, rhc_error_get());
    // cnt: 0, rhc_file_valid: 0, ok: 0, error_msg: file write failed, content invalid
}
