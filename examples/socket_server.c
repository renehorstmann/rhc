#include "rhc/rhc_impl.h"   // once in a project
#include <stdio.h>


int main() {
    puts("creating the server");
    SocketServer server = socketserver_new("127.0.0.1", "12345");

    puts("accepting the client");
    Socket client = socketserver_accept(&server);

    puts("receiving a msg");
    char msg[32];
    socket_recv(&client, msg, 32);
    printf("recv msg: <%32s>\n", msg);

    str_toupper((Str_s) {msg, 32});

    printf("send msg: <%32s>\n", msg);
    socket_send(&client, msg, 32);

    puts("killing sockets");
    socket_kill(&client);
    socketserver_kill(&server);

    puts("finished");
}
