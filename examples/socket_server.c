#include "rhc/rhc_impl.h"   // once in a project
#include <stdio.h>
#include "rhc/socket.h"
#include "rhc/str.h"


int main() {
    puts("creating the server");
    Socket server = socket_new_server("127.0.0.1", "12345");

    puts("accepting the client");
    Socket client = socket_server_accept(&server);

    puts("receiving a msg");
    char msg[32];
    socket_recv(&client, msg, 32);
    printf("recv msg: <%32s>\n", msg);

    str_toupper((Str_s) {msg, 32});

    printf("send msg: <%32s>\n", msg);
    socket_send(&client, msg, 32);

    puts("killing sockets");
    socket_kill(&client);
    socket_kill(&server);

    puts("finished");
}
