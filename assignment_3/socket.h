#ifndef SOCKET_H
#define SOCKET_H

#include <arpa/inet.h>
#include <string.h>

// bind and set up socket
// If there are any errors, returns negative number
int setupSocket();

// Creates a sockaddr_in structure from a socket descriptor,
// port number, and IP address 
struct sockaddr_in createAddr(int, short, in_addr_t);

#endif