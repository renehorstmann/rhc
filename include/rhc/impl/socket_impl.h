#ifndef RHC_SOCKET_IMPL_H
#define RHC_SOCKET_IMPL_H
#ifdef OPTION_SOCKET

#include <assert.h>
#include "../socket.h"
#include "../alloc.h"

#ifdef WIN32
#include <winsock2.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include <windows.h>

typedef struct {
    SOCKET so;
} WinSocket;
_Static_assert(sizeof (WinSocket) <= RHC_SOCKET_STORAGE_SIZE, "storage not big enough");


bool rhc_socketserver_valid(SocketServer self) {
    WinSocket *impl = (WinSocket *) self.impl_storage;
    return impl->so != INVALID_SOCKET;
}

SocketServer rhc_socketserver_new_invalid() {
    SocketServer self = {0};
    WinSocket *impl = (WinSocket *) self.impl_storage;
    impl->so = INVALID_SOCKET;
    return self;
}

SocketServer rhc_socketserver_new(const char *address, const char *port) {
    SocketServer self = {0};
    WinSocket *impl = (WinSocket *) self.impl_storage;

    // winsock startup (can be called multiple times...)
    {
        WORD version = MAKEWORD(2, 2);
        WSADATA wsadata;
        int status = WSAStartup(version, &wsadata);
        if(status != 0) {
            log_error("rhc_socketserver_new failed, WSAStartup failed: &i", status);
            rhc_error = "rhc_socketserver_new failed";
            return rhc_socketserver_new_invalid();
        }
    }

    struct addrinfo hints = {
            .ai_family = AF_UNSPEC,
            .ai_socktype = SOCK_STREAM
    };
//    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    // find a valid address and create a socket on it
    {
        struct addrinfo *servinfo;
        int status = getaddrinfo(address, port, &hints, &servinfo);
        if (status != 0) {
            log_error("rhc_socketserver_new failed: getaddrinfo error: %s\n", gai_strerror(status));
            rhc_error = "rhc_socketserver_new failed";
            return rhc_socketserver_new_invalid();
        }

        for (struct addrinfo *ai = servinfo; ai != NULL; ai = ai->ai_next) {
            impl->so = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
            if (!rhc_socketserver_valid(self))
                continue;

            if(bind(impl->so, ai->ai_addr, (int) ai->ai_addrlen) == -1) {
                rhc_socketserver_kill(&self);
                continue;
            }

            // valid socket + bind
            break;
        }

        freeaddrinfo(servinfo); // free the linked-list
    }

    // no valid address found?
    if(!rhc_socketserver_valid(self)) {
        log_error("rhc_socketserver_new failed to create the server socket");
        rhc_error = "rhc_socketserver_new failed";
        return rhc_socketserver_new_invalid();
    }

    // reuse socket port
    {
        BOOL yes = 1;
        setsockopt(impl->so, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof yes);
    }

    int backlog = 10;   // queue size of incoming connections
    if(listen(impl->so, backlog) == -1) {
        log_error("rhc_socketserver_new failed to listen");
        rhc_error = "rhc_socketserver_new failed";
        rhc_socketserver_kill(&self);
    }

    return self;
}

void rhc_socketserver_kill(SocketServer *self) {
    WinSocket *impl = (WinSocket *) self->impl_storage;
    closesocket(impl->so);
    *self = rhc_socketserver_new_invalid();
}

Socket rhc_socketserver_accept(SocketServer *self) {
    if(!rhc_socketserver_valid(*self))
        return rhc_socket_new_invalid();

    WinSocket *impl = (WinSocket *) self->impl_storage;

    Socket client = {0};
    WinSocket *client_impl = (WinSocket *) client.impl_storage;

    struct sockaddr_storage addr;
    int addrlen = sizeof addr;
    client_impl->so = accept(impl->so, (struct sockaddr *) &addr, &addrlen);

    if(!rhc_socket_valid(client)) {
        log_error("rhc_socketserver_accept failed, killing the server");
        rhc_socketserver_kill(self);
        return rhc_socket_new_invalid();
    }

    char *client_ip = inet_ntoa(((struct sockaddr_in *) &addr)->sin_addr);
    log_info("rhc_socketserver_accept connected with: %s", client_ip);

    return client;
}

bool rhc_socket_valid(Socket self) {
    WinSocket *impl = (WinSocket *) self.impl_storage;
    return impl->so != INVALID_SOCKET;
}

Socket rhc_socket_new_invalid() {
    Socket self = {0};
    WinSocket *impl = (WinSocket *) self.impl_storage;
    impl->so = INVALID_SOCKET;
    return self;
}

Socket rhc_socket_new(const char *address, const char *port) {
    Socket self = {0};
    WinSocket *impl = (WinSocket *) self.impl_storage;

    // winsock startup (can be called multiple times...)
    {
        WORD version = MAKEWORD(2, 2);
        WSADATA wsadata;
        int status = WSAStartup(version, &wsadata);
        if(status != 0) {
            log_error("rhc_socket_new_server failed, WSAStartup failed: &i", status);
            rhc_error = "rhc_socket_new_server failed";
            return rhc_socket_new_invalid();
        }
    }

    struct addrinfo hints = {
            .ai_family = AF_UNSPEC,
            .ai_socktype = SOCK_STREAM
    };

    // find a valid address and connect to it
    {
        struct addrinfo *servinfo;
        int status = getaddrinfo(address, port, &hints, &servinfo);
        if (status != 0) {
            log_error("rhc_socket_new failed: getaddrinfo error: %s\n", gai_strerror(status));
            rhc_error = "rhc_socket_new failed";
            return rhc_socket_new_invalid();
        }

        for (struct addrinfo *ai = servinfo; ai != NULL; ai = ai->ai_next) {
            impl->so = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
            if (!rhc_socket_valid(self))
                continue;

            if(connect(impl->so, ai->ai_addr, (int) ai->ai_addrlen) == -1) {
                rhc_socket_kill(&self);
                continue;
            }

            // valid socket + connect
            break;
        }

        freeaddrinfo(servinfo); // free the linked-list
    }

    // no valid address found?
    if(!rhc_socket_valid(self)) {
        log_error("rhc_socket_new failed to create the connection");
        rhc_error = "rhc_socket_new failed";
        return rhc_socket_new_invalid();
    }

    return self;
}

void rhc_socket_kill(Socket *self) {
    WinSocket *impl = (WinSocket *) self->impl_storage;
    closesocket(impl->so);
    *self = rhc_socket_new_invalid();
}

bool rhc_socket_recv(Socket *self, void *msg, size_t size) {
    if(!rhc_socket_valid(*self))
        false;

    WinSocket *impl = (WinSocket *) self->impl_storage;

    char *buf = msg;
    while(size > 0) {
        int n = recv(impl->so, buf, (int) size, 0);
        if(n <= 0) {
            log_error("rhc_socket_recv failed, killing socket...");
            rhc_socket_kill(self);
            return false;
        }
        assert(n <= size);
        buf += n;
        size -= n;
    }
    return true;
}

bool rhc_socket_send(Socket *self, const void *msg, size_t size) {
    if(!rhc_socket_valid(*self))
        false;

    WinSocket *impl = (WinSocket *) self->impl_storage;

    const char *buf = msg;
    while(size > 0) {
        int n = send(impl->so, buf, (int) size, 0);
        if(n <= 0) {
            log_error("rhc_socket_send failed, killing socket...");
            rhc_socket_kill(self);
            return false;
        }
        assert(n <= size);

        buf += n;
        size -= n;
    }
    return true;
}

#endif


#endif //OPTION_SOCKET
#endif //RHC_SOCKET_IMPL_H
