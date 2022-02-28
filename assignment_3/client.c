/*
 * Assignment 3
 * Client to send requests
 * Pedro Velasquez Duque
 * pv850
 * Due 28/02/2022
 */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include "socket.h"

int main() {
    // define socket
    int clientSocket = setupSocket();
    // check for error
    if(clientSocket < 0) {
        perror("Socket");
        exit(-1);
    }

    // connect socket
    struct sockaddr_in clientAddress = createAddr(clientSocket, 6000, inet_addr("127.0.0.1"));    
    if(connect(clientSocket, (struct sockaddr*)&clientAddress, sizeof(clientAddress)) < 0) {
        perror("Connect");
        exit(-1);
    }

    printf("Connected to server\n");

    // Get user data to send
    char buffer[256];
    while(1) {
        // clear buffer
        bzero(buffer, sizeof(buffer));

        // Get user input
        printf("Enter a message: ");
        fgets(buffer, sizeof(buffer), stdin);
        
        // send data and check for errors
        if(send(clientSocket, buffer, strlen(buffer), 0) < 0) {
            perror("Send");
            close(clientSocket);
            exit(-1);
        }
        
        // If user writes BYE!, client should close connection
        if(strcmp(buffer, "BYE!\n") == 0) {
            close(clientSocket);
            break;
        }
        
        // Reset buffer to receive data
        bzero(buffer, sizeof(buffer));

        // Receive echo from server
        recv(clientSocket, buffer, sizeof(buffer), 0);

        printf("Server response: %s", buffer);
    }

    return 0;
}