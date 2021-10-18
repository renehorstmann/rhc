#ifndef RHC_SOCKET_H
#define RHC_SOCKET_H
#ifdef OPTION_SOCKET

#include <stddef.h>

#define RHC_SOCKET_STORAGE_SIZE 8
#define RHC_SOCKETSERVER_STORAGE_SIZE 8

typedef struct {
    char impl_storage[RHC_SOCKET_STORAGE_SIZE];
} Socket;

typedef struct {
    char impl_storage[RHC_SOCKETSERVER_STORAGE_SIZE];
} SocketServer;


//
// SocketServer
//

// returns true if the SocketServer is valid to use
bool rhc_socketserver_valid(SocketServer self);

// returns a new invalid SocketServer
SocketServer rhc_socketserver_new_invalid();

// Creates a new SocketServer
// address may be "localhost" or "127.0.0.1", to only enable local connections
// address may be "0.0.0.0" to enable all incoming connections
SocketServer rhc_socketserver_new(const char *address, const char *port);

// kills the socketserver and sets it invalid
void rhc_socketserver_kill(SocketServer *self);

// Accepts a new client for a SocketServer
// If an error occures, SocketServer will be set invalid and false is returned
Socket rhc_socketserver_accept(SocketServer *self);


//
// Socket
//

// returns true if the Socket is valid to use
bool rhc_socket_valid(Socket self);
// returns a new invalid Socket
Socket rhc_socket_new_invalid();

// Creates and connects to a server
Socket rhc_socket_new(const char *address, const char *port);

// kills the socket and sets it invalid
void rhc_socket_kill(Socket *self);

// Receives up to size bytes into msg and returns the number of received bytes
// If an error occures, Socket will be set invalid and 0 is returned
size_t rhc_socket_recv(Socket *self, void *msg, size_t size);
// Sends aup to size bytes from msg and returns the number of send bytes
// If an error occures, Socket will be set invalid and 0 is returned
size_t rhc_socket_send(Socket *self, const void *msg, size_t size);

// Receives a full message (blocks until size bytes are received)
// If an error occures, Socket will be set invalid and false is returned
bool rhc_socket_recv_msg(Socket *self, void *msg, size_t size);
// Sends a full message (blocks until size bytes are send)
// If an error occures, Socket will be set invalid and false is returned
bool rhc_socket_send_msg(Socket *self, const void *msg, size_t size);


//
// wrapper without _rhc
//

// returns true if the SocketServer is valid to use
#define socketserver_valid rhc_socketserver_valid

// returns a new invalid SocketServer
#define socketserver_new_invalid rhc_socketserver_new_invalid

// Creates a new SocketServer
// address may be "localhost" or "127.0.0.1", to only enable local connections
// address may be "0.0.0.0" to enable all incoming connections
#define socketserver_new rhc_socketserver_new

// kills the socketserver and sets it invalid
#define socketserver_kill rhc_socketserver_kill

// Accepts a new client for a SocketServer
// If an error occures, SocketServer will be set invalid and false is returned
#define socketserver_accept rhc_socketserver_accept


// returns true if the Socket is valid to use
#define socket_valid rhc_socket_valid
// returns a new invalid Socket
#define socket_new_invalid rhc_socket_new_invalid

// Creates and connects to a server
#define socket_new rhc_socket_new

// kills the socket and sets it invalid
#define socket_kill rhc_socket_kill

// Receives up to size bytes into msg and returns the number of received bytes
// If an error occures, Socket will be set invalid and 0 is returned
#define socket_recv rhc_socket_recv
// Sends aup to size bytes from msg and returns the number of send bytes
// If an error occures, Socket will be set invalid and 0 is returned
#define socket_send rhc_socket_send

// Receives a full message (blocks until size bytes are received)
// If an error occures, Socket will be set invalid and false is returned
#define socket_recv_msg rhc_socket_recv_msg
// Sends a full message (blocks until size bytes are send)
// If an error occures, Socket will be set invalid and false is returned
#define socket_send_msg rhc_socket_send_msg


#endif //OPTION_SOCKET
#endif //RHC_SOCKET_H
