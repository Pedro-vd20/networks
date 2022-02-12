/*
 * Socket programming
 * 
 * 
 * Clients and servers
 *  servers make services available and wait for client requests
 *  may involve replies or not
 * 
 * 
 * Server address
 *  clients must know URL (Universal Resource Locator) and Port number of that service
 *      port 80 is reserved for HTTP
 *      port 21 is reserved for FTP
 * 
 * Sockets and ports
 *  abstraction, unique ID to identify to what application to send data
 *  Stream sockets: use TCP for reliability
 *  Datagram sockets: use UDP for best-effort data
 * 
 *  ports ~ apartment number (when IP is the stree address and you are the application)
 *      there are many applications (people) in an apartment
 *      Ports make sure each person gets their appropriate mail
 * 
 * 
 * Sockets using TCP
 *  Identified but:
 *      Internet address
 *      End-to-end protocol (TCP or UDP)
 *      Port number
 * 
 * 
 * Sockets using UDP
 *  Flow map:
 *      create socket
 *      bind socket to IP
 *      receive data and send data
 *      close the socket
 * 
 * 
 * Basic functions
 *  socket()    creates an instance of socket abstraction
 *      int sockfd = socket(ProtocolFamily, type, protocol)
 * 
 *  bind()
 * 
 * Simple implementation
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

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

    // bind
    struct sockaddr_in srvaddr;
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(5000); // convert from computer byte order to network byte order
    srvaddr.sin_addr.s_addr = INADDR_ANY;

    while(1) {
        char buffer[256];
        scanf("%s", buffer);
        sendto(serverfd, buffer, sizeof(buffer), 0, (const struct sockaddr*)&srvaddr, sizeof(srvaddr));
        if(strcmp(buffer, "exit") == 0) {
            break;
        }
    }    

    close(serverfd);


    return 0;
}