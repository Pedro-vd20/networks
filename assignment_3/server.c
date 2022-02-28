/*
 * Assignment 3
 * Server to receive and fork replies
 * Pedro Velasquez Duque
 * pv850
 * Due 28/02/2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include "socket.h"

int main() {
	// set up socket and check for errors
	int serverSocket = setupSocket();
	if(serverSocket < 0) {
		perror("Socket failed");
		exit(-1);
	}

	// bind socket to listen to all addresses
	struct sockaddr_in serverAddress = createAddr(serverSocket, 6000, INADDR_ANY);
	if(bind(serverSocket, (const struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
		perror("Binding failed");
		exit(-1);
	}

	// listen for new connections
	if(listen(serverSocket, 10) < 0) {
		perror("listen failed");
		close(serverSocket);
		exit(-1);
	}
	
	while(1) {
		// Sender information
		struct sockaddr_in client;
		bzero(&client, sizeof(client));
		socklen_t addrLen = sizeof(client);
		
		// accept connection
		int clientSocket = accept(serverSocket, (struct sockaddr*) &client, &addrLen);
		
        int pid = -1;
		// if connection successful, fork process
		if(clientSocket >= 0) {
            pid = fork(); 
        }
		if(pid == 0) {
		 	char buffer[256];

			// Print connection status
			printf("[%s:%d] Connected\n", inet_ntoa(client.sin_addr), client.sin_port);


			while(1) {
				
				// Clear buffer from previous comm
				bzero(buffer,sizeof(buffer));
				
				// receive data
				int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);

				// print data
				if(bytes != 0) {
					printf("[%s:%d] Received message: %s", inet_ntoa(client.sin_addr), client.sin_port, buffer);
				}

				// Close connection if user sends nothing
				if(bytes == 0 || strcmp(buffer, "BYE!\n") == 0) { 
					printf("[%s:%d] Disconnected\n", inet_ntoa(client.sin_addr), client.sin_port);
					close(clientSocket);
					// end child process
					return 0;
					// Parent loop will never reach here since pid != 0
				}
				
				// Echo data back to client
				send(clientSocket, buffer, strlen(buffer), 0);
			}
		 }
	}


	//recv/send

	//close
	printf("DEAD\n");
	close(serverSocket);
	return 0;
}
