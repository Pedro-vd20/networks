#include "socket.h"

// bind and set up socket
// If there are any errors, returns negative number
int setupSocket() {
    int server = socket(AF_INET, SOCK_STREAM, 0);

    // if binding socket succeeded, make socket reusable
    if (server >= 0 && setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
        return -1; // if socket option fails
    }

    return server;
};

// Creates a sockaddr_in structure from a socket descriptor,
// port number, and IP address
struct sockaddr_in createAddr(int socket, short port, in_addr_t ip) {
    struct sockaddr_in address;
    bzero(&address, sizeof(address)); // set up all bytes to 0

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = ip;

    return address;
}
