#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>

#include "strings.h"
#include "user.h"
#include "threading.h"

/**
 * @brief process http request from a user and reply with http response
 * 
 * @param void* pointer to a client struct holding the socket and address info of a client 
 */
void* makeResponse(void*);

// response building blocks
const char* OK_RESPONSE = "HTTP/1.0 200 ok\r\n";
const char* SERVER = "Server: SimpleHTTPServer";
const char* NOT_UNDERSTOOD = "HTTP/1.0 400 Bad Request\r\n";
const char* FILE_NOT_FOUND = "HTTP/1.0 404 Not Found\r\n";
const char* METHOD_NOT_ALLOWED = "HTTP/1.0 405 Method Not Allowed\r\n";

const char* DATE = "Date: ";
const char* TYPE = "Content-type: ";
const char* LEN = "Content-length: ";
const char* ALLOW = "Allow: GET\r\n";

int main() {

	// set server socket
	int server_sd = socket(AF_INET,SOCK_STREAM,0);
	if(server_sd<0) {
		perror("Socket Failed: ");
		return -1;
	}

	// bind socket
	struct sockaddr_in server_address;	//structure to save IP address and port
	memset(&server_address,0,sizeof(server_address)); //Initialize/fill the server_address to 0
	server_address.sin_family = AF_INET;	//address family
	server_address.sin_port = htons(8080);	//port
	server_address.sin_addr.s_addr = htonl(INADDR_ANY); //htonl(INADDR_LOOPBACK); //inet_addr("127.0.0.1");
	if(bind(server_sd,(struct sockaddr *)&server_address,sizeof(server_address))<0) {
		perror("Bind failed..:");
		return -1;
	}

	// listen on port
	if(listen(server_sd,5)<0) {
		perror("Listen Error:");
		return -1;
	}

	struct sockaddr_in client_address;  //to store the client information

	// multithread addresses to deal with requests
	pthread_t thread_ids[10];
	int busy[10]; // keep track of threads currently in use
	bzero(busy, sizeof(busy)); 

	//4. accept
	while(1) {
		
		// accept new connection from client
		int client_len=sizeof(client_address);	//lent of client cliend address of type sockaddr_in
		int client_sd = accept(server_sd,(struct sockaddr*)&client_address,(socklen_t *)&client_len);	//accept the connection but also fill the client address with client info
		if(client_sd<1)
		{
			perror("Accept Error:");
			return -1;
		}

		// send thread to handle response
		client user;
		user.socket = client_sd;
		user.address = client_address;
		int t_id_index = open_thread(busy, 10);
		// check if index -1 (figure out how to handle later)
		if(pthread_create(thread_ids + t_id_index, NULL, makeResponse, &user) < 0) {
			perror("multithreading");
			return -1;
		}

		// close threads that finish running
		join_thread(thread_ids, busy, 10);
	}	

	close(server_sd);										//close the server socket
	//6. close


}


void* makeResponse(void* arg) {
	
	char* response_code = "200";

	// collect user information
	int sd = ((client*)arg)->socket;
	struct sockaddr_in user_address = ((client*)arg)->address;
	char client_name[100];		//to store client address 
	inet_ntop(AF_INET, &user_address.sin_addr, client_name, sizeof(client_name));
	
	// collect request from client
	char request[1000];
	int bytes = recv(sd, request, sizeof(request), 0);
	char* request_header[3];
	if(parse_header(request_header, request, 1000) < 0 ) {
		// error couldn't understand response

		// ERROR
		// ERROR
		response_code = "400";
	}

	// check request method
	if(strcmp(request_header[0], "GET") != 0) {
		response_code = "405";
		// ERROR
		// ERROR
	}

	// check file type
	char* f_type = get_file_type(request_header[2]);
	if(f_type == NULL) {
		response_code = "400";
		// Error 
		// Error
	}

	// get full file path
	char full_fpath[50] = "files";
	strcat(full_fpath, request_header[1]);

	// open file
	FILE* file = fopen(full_fpath, "r");
	if(file == NULL) {
		response_code = "404";
		// Error 
		// Error
	}

	
	// free header
	for(int i = 0; i < 3; ++i) {
		free(request_header[i]);
	}
	free(f_type);



	// dummy code to delete later
	char response[1000]="HTTP/1.1 200 OK \r\n\n";		//http header

	// FILE* file = fopen("files/index.html","r");				//open the index.html file
	if(file == NULL) {
		perror("File error:");
		
	}

	char line[256];
	while(fgets(line,sizeof(line),file)!=0) {				//concatinate the index.html file line by line into server response
		strcat(response,line);
	}
	fclose(file);
	
	//5. send
	send(sd,response,sizeof(response),0);		//send the server response to the client
	close(sd);									//close the client socket
}