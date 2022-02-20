/*
 * Assignment 2
 * Time and date server
 * Pedro Velasquez Duque
 * pv850
 * Due 21/02/2022  
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>

int setupSocket();
int bindSocket(int, short);

int main() {
    // socket
    int serverSocket = setupSocket();
    if(serverSocket < 0) {
        perror("Socket error");
        return -1;
    }

    // bind socket
    if(bindSocket(serverSocket, 9000) < 0) {
        perror("Socket binding");
        return -1;
    }

    // read everything from the socket and print
    char buffer[256];
    // keep track of sender's address info
    struct sockaddr_in senderAddress;
    socklen_t addrLen = sizeof(senderAddress);

    while(1) {
        // clear buffers and data
        // bzero(buffer, sizeof(buffer));
        bzero(&senderAddress, sizeof(senderAddress));

        // Collect data
        recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&senderAddress, &addrLen);

        // print IP and port
        printf("Time request from: %s:%hu\n", inet_ntoa(senderAddress.sin_addr), senderAddress.sin_port);

        // collect data and time
        time_t t = time(NULL);
        char date[64] = "Server time: ";
        strcat(date, asctime(localtime(&t)));
        
        // return time to the client
        sendto(serverSocket, date, strlen(date), 0, (const struct sockaddr*)&senderAddress, sizeof(senderAddress));
    }

    close(serverSocket);

    return 0;
}

// bind and set up socket
// If there are any errors, returns negative number
int setupSocket() {
    int server = socket(AF_INET, SOCK_DGRAM, 0);

    // if binding socket succeeded
    if(server >= 0) {
        // make socket reusable by client
        if(setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
            return -1;  // if socket option fails
        }
    }

    return server;
};


// binds a socket to a port
// returns negative number if there is an error
int bindSocket(int socket, short port) {
    struct sockaddr_in serverAddress;
    bzero(&serverAddress, sizeof(serverAddress)); // set up all bytes to 0

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // pass struct to bind function
    return bind(socket, (const struct sockaddr*)&serverAddress, sizeof(serverAddress));
}