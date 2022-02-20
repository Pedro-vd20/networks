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
    if(connect(server_sd, (struct sockaddr*)&srvaddr, sizeof(srvaddr)) < 0) {
        perror("Connect");
        exit(-1);
    }


    // receive recv / send
    char buffer[256];
    while(1) {
        fgets(buffer, sizeof(buffer), stdin);
        if(strcmp(buffer, "bye\n") == 0) {
            send(server_sd, buffer, 0, 0);
            close(server_sd);
            break;
        }
        if(send(server_sd, buffer, strlen(buffer), 0) < 0) {
            perror("Send");
            close(server_sd);
            exit(-1);
        }
        bzero(buffer, sizeof(buffer));
    }

    // close socket
    close(server_sd);

    return 0;
}