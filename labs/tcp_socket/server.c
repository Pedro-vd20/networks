#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    
    // define socket
    int server_sd = socket(AF_INET, SOCK_STREAM, 0);
    // check for error
    if(server_sd < 0) {
        perror("Socket");
        exit(-1);
    }
    // let socket be reused
    if(setsockopt(server_sd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
        perror("Socket reuse");
        return -1;
    }

    // bind socket
    struct sockaddr_in srvaddr;
    bzero(&srvaddr, sizeof(srvaddr)); // fill with 0s
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(5000);
    srvaddr.sin_addr.s_addr = inet_addr("127.0.0.1");   /* loopback (local) address
                                                        INADDR_LOOPBACK */
    int status = bind(server_sd, (struct sockaddr*)&srvaddr, sizeof(srvaddr));
    // check errors
    if(status < 0) {
        perror("Bind");
        exit(-1);
    }

    // listen on socket
    if(listen(server_sd, 5) < 0) {
        perror("Listen");
        close(server_sd);
        exit(-1);
    }

    // accept a connection
    char buffer[256];
    printf("Server is listening...\n");
    while(1) {
        int client_sd = accept(server_sd, 0, 0);
        // error check
        if(client_sd < 0) {
            perror("Accept");
            close(server_sd);
            exit(-1);
        }
        fork();

        while(1) {
            bzero(buffer, sizeof(buffer));
            // receive recv / send
            int bytes = recv(client_sd, buffer, sizeof(buffer), 0);
            if(bytes <= 0) {
                close(client_sd);
                break;
            }
            printf("%s", buffer);
        }
    }

    // receive recv / send

    // close socket
    close(server_sd);

    return 0;
}