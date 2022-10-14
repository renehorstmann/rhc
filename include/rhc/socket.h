#ifndef RHC_SOCKET_H
#define RHC_SOCKET_H
#ifdef OPTION_SOCKET

//
// TCP sockets
//

#include "stream.h"

typedef struct RhcSocket RhcSocket;

typedef struct RhcSocketServer RhcSocketServer;


//
// SocketServer
//

// returns true if the SocketServer is valid to use
RHC_EXPORT
bool rhc_socketserver_valid(const RhcSocketServer *self);

// returns a new invalid SocketServer
static RhcSocketServer *rhc_socketserver_new_invalid() {
    return NULL;
}

// Creates a new SocketServer
// address may be "localhost" or "127.0.0.1", to only enable local connections
// address may be "0.0.0.0" to enable all incoming connections
// if address is NULL, "127.0.0.1" is used
// SDLs implementation is only able to use "0.0.0.0"
RHC_EXPORT
RhcSocketServer *rhc_socketserver_new(const char *address, rhcu16 port);

// kills the socketserver and sets it invalid
RHC_EXPORT
void rhc_socketserver_kill(RhcSocketServer **self_ptr);

// Accepts a new client for a SocketServer
// If an error occurs, SocketServer will be set invalid and false is returned
RHC_EXPORT
RhcSocket *rhc_socketserver_accept(RhcSocketServer *self);



//
// Socket
//

// returns true if the Socket is valid to use
RHC_EXPORT
bool rhc_socket_valid(const RhcSocket *self);

// returns a new invalid Socket
static RhcSocket *rhc_socket_new_invalid() {
    return NULL;
}

// Creates and connects to a server
// if address is NULL, "127.0.0.1" is used
// not for emscripten users: compile with -s ASYNCIFY=1, because emscripten_sleep(100); will be called
RHC_EXPORT
RhcSocket *rhc_socket_new(const char *address, rhcu16 port);


// Sets the timeout in ms for recv and send
// not supported for sdl (which will log a warning)
RHC_EXPORT
void rhc_socket_set_timeout(RhcSocket *self, int timeout_ms);


// kills the socket and sets it invalid
RHC_EXPORT
void rhc_socket_kill(RhcSocket **self_ptr);


// returns the stream for the socket
RHC_EXPORT
RhcStream_i rhc_socket_get_stream(RhcSocket *self);


#endif //OPTION_SOCKET
#endif //RHC_SOCKET_H
