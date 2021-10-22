#include "rhc/rhc_impl.h"   // once in a project
#include <stdio.h>

int main() {
    puts("creating the connection");
    Socket *socket = socket_new("127.0.0.1", 12345);

    // returns socket->stream or an invalid Stream_i, if socket is NULL
    Stream_i ss = socket_get_stream(socket);

    char msg[32];
    sprintf(msg, "Hello Server");
    printf("write msg: <%32s>\n", msg);
    stream_write_msg(ss, msg, 32);

    puts("reading");
    stream_read_msg(ss, msg, 32);
    printf("read msg: <%32s>\n", msg);

    puts("killing socket");
    socket_kill(&socket);

    puts("finished");
}
