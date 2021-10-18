#include "rhc/rhc_impl.h"   // once in a project
#include <stdio.h>

int main() {
    puts("creating the connection");
    Socket socket = socket_new("127.0.0.1", "12345");

    char msg[32];
    sprintf(msg, "Hello Server");
    printf("send msg: <%32s>\n", msg);
    socket_send_msg(&socket, msg, 32);

    puts("receiving");
    socket_recv_msg(&socket, msg, 32);
    printf("recv msg: <%32s>\n", msg);

    puts("killing socket");
    socket_kill(&socket);

    puts("finished");
}
