#include "rhc/rhc_impl.h"   // once in a project
#include <stdio.h>


int main() {
    puts("creating the server");
    SocketServer server = socketserver_new("127.0.0.1", 12345);

    puts("accepting the client");
    Socket *client = socketserver_accept(&server);

    // returns client->stream, or an invalid Stream_i, if client is NULL
    Stream_i cs = socket_get_stream(client);

    puts("read a msg");
    char msg[32];
    stream_read_msg(cs, msg, 32);
    printf("read msg: <%32s>\n", msg);

    str_toupper((Str_s) {msg, 32});

    printf("write msg: <%32s>\n", msg);
    stream_write_msg(cs, msg, 32);

    puts("killing sockets");
    socket_kill(&client);
    socketserver_kill(&server);

    puts("finished");
}
