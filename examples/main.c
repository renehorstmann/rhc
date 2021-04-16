#include "rhc/rhc_impl.h"


int main() {

    String f = file_read("../examples/main.c", FILE_READ_MODE_ASCII);

    if(String_valid(f))
        printf("file:\n%s", f.s.data);

    String_delete(&f);
}

