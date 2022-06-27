#include "rhc/rhc_impl.h"   // once in a project
#include <stdio.h>


int main() {
    puts("creating the server");
    RhcSocketServer *server = rhc_socketserver_new("127.0.0.1", 12345);

    puts("accepting the client");
    RhcSocket *client = rhc_socketserver_accept(server);

    // returns client->stream, or an invalid RhcStream_i, if client is NULL
    RhcStream_i cs = rhc_socket_get_stream(client);

    puts("read a msg");
    char msg[32];
    rhc_stream_read(cs, msg, 32);
    printf("read msg: <%32s>\n", msg);

    rhc_str_toupper((RhcStr_s) {msg, 32});

    printf("write msg: <%32s>\n", msg);
    rhc_stream_write(cs, msg, 32);

    puts("killing sockets");
    rhc_socket_kill(&client);
    rhc_socketserver_kill(&server);

    puts("finished");
}
