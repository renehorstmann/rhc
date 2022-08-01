#ifndef RHC_SOCKET_IMPL_H
#define RHC_SOCKET_IMPL_H
#ifdef RHC_IMPL
#ifdef OPTION_SOCKET

#include "../socket.h"


//
// sdl
//
#ifdef OPTION_SDL
#ifdef PLATFORM_EMSCRIPTEN
#include <emscripten.h>
#endif
#include <limits.h>
#include "SDL2/SDL_net.h"

struct RhcSocket {
    TCPsocket so;
};
struct RhcSocketServer {
    TCPsocket so;
};

static void rhc__socket_close(RhcSocket *self) {
    if(!rhc_socket_valid(self))
        return;
    SDLNet_TCP_Close(self->so);
    self->so = NULL;
}

static void rhc__socketserver_close(RhcSocketServer *self) {
    if(!rhc_socketserver_valid(self))
        return;
    SDLNet_TCP_Close(self->so);
    self->so = NULL;
}


static rhcsize rhc__socket_read(struct RhcStream_i stream, void *memory, rhcsize n) {
    RhcSocket *self = stream.impl;
    if(!rhc_socket_valid(self))
        return 0;

    rhcsize read = SDLNet_TCP_Recv(self->so, memory, n);
    if(read <= 0) {
        rhc_log_error("rhc_socket_recv failed, killing socket...");
        rhc__socket_close(self);
        return 0;
    }
    assert(read <= n);
    return read;
}

static rhcsize rhc__socket_write(struct RhcStream_i stream, const void *memory, rhcsize n) {
    RhcSocket *self = stream.impl;
    if(!rhc_socket_valid(self))
        return 0;

    rhcsize written = SDLNet_TCP_Send(self->so, memory, n);
    if(written <= 0) {
        rhc_log_error("rhc_socket_send failed, killing socket...");
        rhc__socket_close(self);
        return 0;
    }
    assert(written <= n);
    return written;
}

//
// public
//


bool rhc_socket_valid(const RhcSocket *self) {
    return self && self->so!=NULL;
}

bool rhc_socketserver_valid(const RhcSocketServer *self) {
    return self && self->so!=NULL;
}

RhcSocketServer *rhc_socketserver_new(const char *address, rhcu16 port) {
    RhcSocketServer *self = rhc_new0(RhcSocketServer, 1);

    if(address && strcmp(address, "0.0.0.0") != 0) {
        rhc_log_warn("rhc_socketserver_new SDLNet uses always a public server (0.0.0.0)");
    }

    IPaddress ip;
    if(SDLNet_ResolveHost(&ip, NULL, port) == -1) {
        rhc_log_error("rhc_socketserver_new failed to resolve host: %s", SDLNet_GetError());
        rhc_error_set("rhc_socketserver_new failed");
        rhc_free(self);
        return rhc_socketserver_new_invalid();
    }

    self->so = SDLNet_TCP_Open(&ip);
    // impl->so will be NULL on error, so _valid check would fail

    if(!rhc_socketserver_valid(self)) {
        rhc_log_error("rhc_socketserver_new failed to create the server socket");
        rhc_error_set("rhc_socketserver_new failed");
        rhc_free(self);
        return rhc_socketserver_new_invalid();
    }

    return self;
}

RhcSocket *rhc_socketserver_accept(RhcSocketServer *self) {
    if(!rhc_socketserver_valid(self))
        return rhc_socket_new_invalid();

    RhcSocket *client = rhc_new0(RhcSocket, 1);

    for(;;) {
        client->so = SDLNet_TCP_Accept(self->so);
        if(client->so)
            break;
        SDL_Delay(50); // sleep some millis
    }

    if(!rhc_socket_valid(client)) {
        rhc_log_error("rhc_socketserver_accept failed, killing the server");
        rhc_socketserver_kill(&self);
        rhc_free(client);
        return rhc_socket_new_invalid();
    }

    IPaddress *client_ip = SDLNet_TCP_GetPeerAddress(client->so);
    if(!client_ip) {
        rhc_log_warn("rhc_socketserver_accept failed to get client ip address");
    } else {
        rhc_log_info("rhc_socketserver_accept connected with: %s", SDLNet_ResolveIP(client_ip));
    }

    return client;
}


RhcSocket *rhc_socket_new(const char *address, rhcu16 port) {
    RhcSocket *self = rhc_new0(RhcSocket, 1);

    if(!address)
        address = "127.0.0.1";

    IPaddress ip;
    if(SDLNet_ResolveHost(&ip, address, port) == -1) {
        rhc_log_error("rhc_socketserver_new failed to resolve host: %s", SDLNet_GetError());
        rhc_error_set("rhc_socketserver_new failed");
        return rhc_socket_new_invalid();
    }

    self->so = SDLNet_TCP_Open(&ip);
    // impl->so will be NULL on error, so _valid check would fail

    if(!rhc_socket_valid(self)) {
        rhc_log_error("rhc_socket_new failed to create the connection");
        rhc_error_set("rhc_socket_new failed");
        rhc_free(self);
        return rhc_socket_new_invalid();
    }

#ifdef PLATFORM_EMSCRIPTEN
    if(rhc_socket_valid(self)) {
        emscripten_sleep(100);  // sleep and let the connection be opened (blocks, but runs the event loop)
    }
#endif

    return self;
}

#else

//
// UNIX
//
#ifdef PLATFORM_UNIX
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

struct RhcSocket {
    int so;
};
struct RhcSocketServer {
    int so;
};

static void rhc__socket_close(RhcSocket *self) {
    if(!rhc_socket_valid(self))
        return;
    close(self->so);
    self->so = -1;
}

static void rhc__socketserver_close(RhcSocketServer *self) {
    if(!rhc_socketserver_valid(self))
        return;
    close(self->so);
    self->so = -1;
}

static rhcsize rhc__socket_read(struct RhcStream_i stream, void *memory, rhcsize n) {
    RhcSocket *self = stream.impl;
    if(!rhc_socket_valid(self))
        return 0;

    rhcsize read = recv(self->so, memory, n, MSG_NOSIGNAL);
    if(read <= 0) {
        rhc_log_error("rhc_socket_recv failed, killing socket...");
        rhc__socket_close(self);
        return 0;
    }
    assert(read <= n);
    return read;
}

static rhcsize rhc__socket_write(struct RhcStream_i stream, const void *memory, rhcsize n) {
    RhcSocket *self = stream.impl;
    if(!rhc_socket_valid(self))
        return 0;

    rhcsize written = send(self->so, memory, n, MSG_NOSIGNAL);
    if(written <= 0) {
        rhc_log_error("rhc_socket_send failed, killing socket...");
        rhc__socket_close(self);
        return 0;
    }
    assert(written <= n);
    return written;
}

//
// public
//

bool rhc_socket_valid(const RhcSocket *self) {
    return self && self->so>=0;
}

bool rhc_socketserver_valid(const RhcSocketServer *self) {
    return self && self->so >= 0;
}

RhcSocketServer *rhc_socketserver_new(const char *address, rhcu16 port) {
    RhcSocketServer *self = rhc_new0(RhcSocketServer, 1);

    if(!address)
        address = "127.0.0.1";

    char port_str[8];
    snprintf(port_str, 8, "%i", port);

    struct addrinfo hints = {
            .ai_family = AF_UNSPEC,
            .ai_socktype = SOCK_STREAM
    };
//    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    // find a valid address and create a socket on it
    {
        struct addrinfo *servinfo;
        int status = getaddrinfo(address, port_str, &hints, &servinfo);
        if (status != 0) {
            rhc_log_error("rhc_socketserver_new failed: getaddrinfo error: %s\n", gai_strerror(status));
            rhc_error_set("rhc_socketserver_new failed");
            return rhc_socketserver_new_invalid();
        }

        for (struct addrinfo *ai = servinfo; ai != NULL; ai = ai->ai_next) {
            self->so = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
            if (!rhc_socketserver_valid(self))
                continue;

            if(bind(self->so, ai->ai_addr, (int) ai->ai_addrlen) == -1) {
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
        rhc_log_error("rhc_socketserver_new failed to create the server socket");
        rhc_error_set("rhc_socketserver_new failed");
        return rhc_socketserver_new_invalid();
    }

    // reuse socket port
    {
        int yes = 1;
        setsockopt(self->so, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
    }

    int backlog = 10;   // queue size of incoming connections
    if(listen(self->so, backlog) == -1) {
        rhc_log_error("rhc_socketserver_new failed to listen");
        rhc_error_set("rhc_socketserver_new failed");
        rhc_socketserver_kill(&self);
    }

    return self;
}

RhcSocket *rhc_socketserver_accept(RhcSocketServer *self) {
    if(!rhc_socketserver_valid(self))
        return rhc_socket_new_invalid();

    RhcSocket *client = rhc_new0(RhcSocket, 1);

    struct sockaddr_storage addr;
    socklen_t addrlen = sizeof addr;
    client->so = accept(self->so, (struct sockaddr *) &addr, &addrlen);

    if(!rhc_socket_valid(client)) {
        rhc_log_error("rhc_socketserver_accept failed, killing the server");
        rhc_socketserver_kill(&self);
        rhc_free(client);
        return rhc_socket_new_invalid();
    }

    char *client_ip = inet_ntoa(((struct sockaddr_in *) &addr)->sin_addr);
    rhc_log_info("rhc_socketserver_accept connected with: %s", client_ip);

    return client;
}

RhcSocket *rhc_socket_new(const char *address, rhcu16 port) {
    RhcSocket *self = rhc_new0(RhcSocket, 1);

    if(!address)
        address = "127.0.0.1";

    char port_str[8];
    snprintf(port_str, 8, "%i", port);

    struct addrinfo hints = {
            .ai_family = AF_UNSPEC,
            .ai_socktype = SOCK_STREAM
    };

    // find a valid address and connect to it
    {
        struct addrinfo *servinfo;
        int status = getaddrinfo(address, port_str, &hints, &servinfo);
        if (status != 0) {
            rhc_log_error("rhc_socket_new failed: getaddrinfo error: %s\n", gai_strerror(status));
            rhc_error_set("rhc_socket_new failed");
            rhc_free(self);
            return rhc_socket_new_invalid();
        }

        for (struct addrinfo *ai = servinfo; ai != NULL; ai = ai->ai_next) {
            self->so = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
            if (!rhc_socket_valid(self))
                continue;

            if(connect(self->so, ai->ai_addr, (int) ai->ai_addrlen) == -1) {
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
        rhc_log_error("rhc_socket_new failed to create the connection");
        rhc_error_set("rhc_socket_new failed");
        rhc_free(self);
        return rhc_socket_new_invalid();
    }
    return self;
}

#endif // PLATFORM_UNIX


//
// mingw - windows
//
#if defined(PLATFORM_MINGW) || defined(PLATFORM_MSVC)
#include <winsock2.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include <windows.h>



struct RhcSocket {
    SOCKET so;
};
struct RhcSocketServer {
    SOCKET so;
};

static void rhc__socket_close(RhcSocket *self) {
    if(!rhc_socket_valid(self))
        return;
    closesocket(self->so);
    self->so = INVALID_SOCKET;
}

static void rhc__socketserver_close(RhcSocketServer *self) {
    if(!rhc_socketserver_valid(self))
        return;
    closesocket(self->so);
    self->so = INVALID_SOCKET;
}

static rhcsize rhc__socket_read(struct RhcStream_i stream, void *memory, rhcsize n) {
    RhcSocket *self = stream.impl;
    if(!rhc_socket_valid(self))
        return 0;

    rhcsize read = recv(self->so, memory, (int) n, 0);
    if(read <= 0) {
        rhc_log_error("rhc_socket_recv failed, killing socket...");
        rhc__socket_close(self);
        return 0;
    }
    assert(read <= n);
    return read;
}

static rhcsize rhc__socket_write(struct RhcStream_i stream, const void *memory, rhcsize n) {
    RhcSocket *self = stream.impl;
    if(!rhc_socket_valid(self))
        return 0;

    rhcsize written = send(self->so, memory, (int) n, 0);
    if(written <= 0) {
        rhc_log_error("rhc_socket_send failed, killing socket...");
        rhc__socket_close(self);
        return 0;
    }
    assert(written <= n);
    return written;
}

//
// public
//

bool rhc_socket_valid(const RhcSocket *self) {
    return self && self->so!=INVALID_SOCKET;
}

bool rhc_socketserver_valid(const RhcSocketServer *self) {
    return self && self->so!=INVALID_SOCKET;
}


RhcSocketServer *rhc_socketserver_new(const char *address, rhcu16 port) {
    RhcSocketServer *self = rhc_new0(RhcSocketServer, 1);

    if(!address)
        address = "127.0.0.1";

    char port_str[8];
    snprintf(port_str, 8, "%i", port);

    // winsock startup (can be called multiple times...)
    {
        WORD version = MAKEWORD(2, 2);
        WSADATA wsadata;
        int status = WSAStartup(version, &wsadata);
        if(status != 0) {
            rhc_log_error("rhc_socketserver_new failed, WSAStartup failed: &i", status);
            rhc_error_set("rhc_socketserver_new failed");
            rhc_free(self);
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
        int status = getaddrinfo(address, port_str, &hints, &servinfo);
        if (status != 0) {
            rhc_log_error("rhc_socketserver_new failed: getaddrinfo error: %s\n", gai_strerror(status));
            rhc_error_set("rhc_socketserver_new failed");
            return rhc_socketserver_new_invalid();
        }

        for (struct addrinfo *ai = servinfo; ai != NULL; ai = ai->ai_next) {
            self->so = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
            if (!rhc_socketserver_valid(self))
                continue;

            if(bind(self->so, ai->ai_addr, (int) ai->ai_addrlen) == -1) {
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
        rhc_log_error("rhc_socketserver_new failed to create the server socket");
        rhc_error_set("rhc_socketserver_new failed");
        return rhc_socketserver_new_invalid();
    }

    // reuse socket port
    {
        BOOL yes = 1;
        setsockopt(self->so, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof yes);
    }

    int backlog = 10;   // queue size of incoming connections
    if(listen(self->so, backlog) == -1) {
        rhc_log_error("rhc_socketserver_new failed to listen");
        rhc_error_set("rhc_socketserver_new failed");
        rhc_socketserver_kill(&self);
    }

    return self;
}


RhcSocket *rhc_socketserver_accept(RhcSocketServer *self) {
    if(!rhc_socketserver_valid(self))
        return rhc_socket_new_invalid();

    RhcSocket *client = rhc_new0(RhcSocket, 1);

    struct sockaddr_storage addr;
    socklen_t addrlen = sizeof addr;
    client->so = accept(self->so, (struct sockaddr *) &addr, &addrlen);

    if(!rhc_socket_valid(client)) {
        rhc_log_error("rhc_socketserver_accept failed, killing the server");
        rhc_socketserver_kill(&self);
        rhc_free(client);
        return rhc_socket_new_invalid();
    }

    char *client_ip = inet_ntoa(((struct sockaddr_in *) &addr)->sin_addr);
    rhc_log_info("rhc_socketserver_accept connected with: %s", client_ip);

    return client;
}


RhcSocket *rhc_socket_new(const char *address, rhcu16 port) {
    RhcSocket *self = rhc_new0(RhcSocket, 1);

    if(!address)
        address = "127.0.0.1";

    char port_str[8];
    snprintf(port_str, 8, "%i", port);

    // winsock startup (can be called multiple times...)
    {
        WORD version = MAKEWORD(2, 2);
        WSADATA wsadata;
        int status = WSAStartup(version, &wsadata);
        if(status != 0) {
            rhc_log_error("rhc_socket_new_server failed, WSAStartup failed: &i", status);
            rhc_error_set("rhc_socket_new_server failed");
            rhc_free(self);
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
        int status = getaddrinfo(address, port_str, &hints, &servinfo);
        if (status != 0) {
            rhc_log_error("rhc_socket_new failed: getaddrinfo error: %s\n", gai_strerror(status));
            rhc_error_set("rhc_socket_new failed");
            rhc_free(self);
            return rhc_socket_new_invalid();
        }

        for (struct addrinfo *ai = servinfo; ai != NULL; ai = ai->ai_next) {
            self->so = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
            if (!rhc_socket_valid(self))
                continue;

            if(connect(self->so, ai->ai_addr, (int) ai->ai_addrlen) == -1) {
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
        rhc_log_error("rhc_socket_new failed to create the connection");
        rhc_error_set("rhc_socket_new failed");
        rhc_free(self);
        return rhc_socket_new_invalid();
    }
    return self;
}
#endif // Windows

#endif //OPTION_SDL



//
// for all:
//

static bool rhc__socket_valid(struct RhcStream_i stream) {
    RhcSocket *self = stream.impl;
    return rhc_socket_valid(self);
}

void rhc_socketserver_kill(RhcSocketServer **self_ptr) {
    RhcSocketServer *self = *self_ptr;
    rhc__socketserver_close(self);
    rhc_free(self);
    *self_ptr = NULL;
}

void rhc_socket_kill(RhcSocket **self_ptr) {
    RhcSocket *self = *self_ptr;
    rhc__socket_close(self);
    rhc_free(self);
    *self_ptr = NULL;
}

RhcStream_i rhc_socket_get_stream(RhcSocket *self) {
    return (RhcStream_i) {
            .impl = self,
            .opt_read_try = rhc__socket_read,
            .opt_write_try = rhc__socket_write,
            .valid = rhc__socket_valid
    };
}


#endif //OPTION_SOCKET
#endif //RHC_IMPL
#endif //RHC_SOCKET_IMPL_H
