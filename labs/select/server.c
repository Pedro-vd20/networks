#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/select.h>

#ifndef FD_COPY
#define FD_COPY(dest, src) memcpy((dest), (src), sizeof *(dest)) 
#endif


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

    fd_set full_fdset;
    fd_set read_fdset;

    int max_fd = server_sd;

    FD_SET(server_sd, &full_fdset);

    // accept a connection
    printf("Server is listening...\n");
    while(1) {
        FD_COPY(&full_fdset, &read_fdset);

        if(select(max_fd + 1, &read_fdset, NULL, NULL, NULL)<0) {
            perror("select");
            return -1;
        }

        for(int fd = 3; fd <= max_fd; fd++) {
            if(FD_ISSET(fd, &read_fdset)) {
                // if activity is on server side
                if(fd == server_sd) {
                    // new connection accepted
                    int client_sd = accept(server_sd, 0, 0);
                    printf("CLiend fd = %d \n", client_sd);

                    // set connection to list of monitored connections
                    FD_SET(client_sd, &full_fdset);
                    // update max if needed
                    if(client_sd > max_fd) {
                        max_fd = client_sd;
                    }
                }
                // if activity not on server
                else {
                    char buffer[256];
                    bzero(buffer, sizeof(buffer));
                    int bytes = recv(fd, buffer, sizeof(buffer), 0);

                    if(bytes == 0) {
                        printf("Connection closed from client side\n");
                        close(fd);
                        FD_CLR(fd, &full_fdset);

                        if(fd == max_fd) {
                            for(int i = max_fd; i >=3; --i) {
                                if(FD_ISSET(i, &full_fdset)) {
                                    max_fd = i;
                                    break;
                                }
                            }
                        }
                    }

                    printf("%s\n", buffer);
                }

            }
        }

        /* int client_sd = accept(server_sd, 0, 0);
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
        } */
    }

    // receive recv / send

    // close socket
    close(server_sd);

    return 0;
}