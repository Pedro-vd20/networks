/*
 * Assignment 2
 * Client time request file
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

int main() {
    // set up socket and handle errors 
    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if(clientSocket < 0) {
        perror("Socket error");
        return -1;
    }

    // set up address
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(9000); 
    address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // send request to server
    char* message = "Time request";
    sendto(clientSocket, message, strlen(message), 0, (const struct sockaddr*)&address, sizeof(address));

    // listen for response
    char time[64];
    recvfrom(clientSocket, time, sizeof(time), 0, 0, 0);

    printf("%s\n", time);


    close(clientSocket);


    return 0;
}