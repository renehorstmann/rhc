#include "rhc/rhc_impl.h"   // once in a project
#include <stdio.h>

int main() {
    puts("creating the connection");
    RhcSocket *socket = rhc_socket_new("127.0.0.1", 12345);

    // returns socket->stream or an invalid RhcStream_i, if socket is NULL
    RhcStream_i ss = rhc_socket_get_stream(socket);

    char msg[32] = {0};
    sprintf(msg, "Hello Server");
    printf("write msg: <%32s>\n", msg);
    rhc_stream_write(ss, msg, 32);

    puts("reading");
    rhc_stream_read(ss, msg, 32);
    printf("read msg: <%32s>\n", msg);

    puts("killing socket");
    rhc_socket_kill(&socket);

    puts("finished");
}
