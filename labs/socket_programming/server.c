#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

/* 
 * Steps from socket
 * socket()
 * bind()
 * recvfrom()
 * close()
 */

int main() {
    // socket
    int serverfd = socket(AF_INET, SOCK_DGRAM, 0);
        // AF_INET internet protocols version 4
        // SOCK_DGRAM socket datagram (for UDP) 
    // handle function errors (socket returns negative number)
    if(serverfd < 0) {
        perror("");
        return -1;
    }
    if(setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
        perror("");
        return -1;
    }
    // bind
    struct sockaddr_in srvaddr;
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(5000); // convert from computer byte order to network byte order
    srvaddr.sin_addr.s_addr = INADDR_ANY;
    // pass struct to bind function
    int status = bind(serverfd, (const struct sockaddr*)&srvaddr, sizeof(srvaddr));
    // check if bind fail
    if(status < 0) {
        perror("");
        return -1;
    }

    // read everything from the socket and print
    while(1) {
        // to read UDP, use recvfrom
        char buffer[256];
        recvfrom(serverfd, buffer, sizeof(buffer), 0, 0, 0);
        printf("%s \n", buffer);
    }

    close(serverfd);

    return 0;
}