#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "strings.h"
#include "user.h"
#include "threading.h"

/**
 * @brief process http request from a user and reply with http response
 * 
 * @param void* pointer to a client struct holding the socket and address info of a client 
 */
void* make_response(void*);

/**
 * @brief builds an http response based on the provided status code
 * 
 * @param char* status code of message
 * @param char* file type (null if error status)
 * @param char* http body (null if error message)
 * @return char* full http response (using malloc, caller must free memory)
 */
char* build_response(char*, char*, char*, struct tm*);

/**
 * @brief print out info about request
 * 
 * @param char* ip
 * @param struct tm* local time of request
 * @param char* line of request
 * @param char* status code
 */
void debug(char*, struct tm*, char*, char*);

// response building blocks
const char* OK_RESPONSE = "HTTP/1.0 200 ok\r\n";
const char* SERVER = "Server: SimpleHTTPServer\r\n";
const char* NOT_UNDERSTOOD = "HTTP/1.0 400 Bad Request\r\n";
const char* FILE_NOT_FOUND = "HTTP/1.0 404 Not Found\r\n";
const char* METHOD_NOT_ALLOWED = "HTTP/1.0 405 Method Not Allowed\r\n";

const char* DATE = "Date: ";
const char* TYPE = "Content-type: ";
const char* LEN = "Content-length: ";
const char* ALLOW = "Allow: GET\r\n";
const char* NOT_FOUND_BODY = "<html>\n\t<head>\n\t\t<title>404 Page Not Found</title>\n\t</head>\n\t<body>\n\t\t<h1>404 Page Not Found</h1>\n\t</body>\n</html>";
const char* NOT_FOUND_LENGTH = "114";

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
		if(pthread_create(thread_ids + t_id_index, NULL, make_response, &user) < 0) {
			perror("multithreading");
			return -1;
		}

		// close threads that finish running
		join_thread(thread_ids, busy, 10);
	}	

	close(server_sd);										//close the server socket
	//6. close

	return 0;


}


void* make_response(void* arg) {

	// collect user information
	int sd = ((client*)arg)->socket;
	struct sockaddr_in user_address = ((client*)arg)->address;
	char client_name[100];		//to store client address 
	inet_ntop(AF_INET, &user_address.sin_addr, client_name, sizeof(client_name));
	
	// get time
	time_t t = time(NULL);
	struct tm* now = localtime(&t); 

	// collect request from client
	char request[1000];
	int bytes = recv(sd, request, sizeof(request), 0);
	char first_line[100];
	get_first_line(request, first_line);

	char* request_header[3];
	if(parse_header(request_header, first_line) < 0 ) {
		char* response = build_response("400", NULL, NULL, now);
		debug(client_name, now, first_line, "400");

		// send response
		send(sd, response, strlen(response), 0);
		close(sd);

		// free used memory
		free(response);
		for(int i = 0; i < 3; ++i) {
			free(request_header[i]);
		}

		return;
	}

	// check request method
	if(strcmp(request_header[0], "GET") != 0) {
		char* response = build_response("405", NULL, NULL, now);
		debug(client_name, now, first_line, "405");

		// send response
		send(sd, response, strlen(response), 0);
		close(sd);

		// free used memory
		free(response);
		for(int i = 0; i < 3; ++i) {
			free(request_header[i]);
		}

		return;
	}

	// check file type
	char* f_type = get_file_type(request_header[1]);
	if(f_type == NULL) {
		char* response = build_response("404", NULL, NULL, now);
		debug(client_name, now, first_line, "404");
		
		// send response
		send(sd, response, strlen(response), 0);
		close(sd);

		// free used memory
		free(f_type);
		free(response);
		for(int i = 0; i < 3; ++i) {
			free(request_header[i]);
		}

		return;
	}

	// get full file path
	char full_fpath[50] = "files";
	strcat(full_fpath, request_header[1]);

	// open file
	FILE* file = fopen(full_fpath, "r");
	if(file == NULL) {
		char* response = build_response("404", NULL, NULL, now);
		debug(client_name, now, first_line, "404");

		// send response
		send(sd, response, strlen(response), 0);
		close(sd);

		// free used memory
		free(f_type);
		free(response);
		for(int i = 0; i < 3; ++i) {
			free(request_header[i]);
		}

		return;
	}

	// get file contents
	char* file_content = malloc(50000);
	bzero(file_content, sizeof(file_content));
	char line[256];
	while(fgets(line, sizeof(line), file) != 0) {
		strcat(file_content, line);
	}
	fclose(file);

	// build HTTP response
	char* response = build_response("200", f_type, file_content, now);
	debug(client_name, now, first_line, "200");

	// send response
	send(sd, response, strlen(response), 0);
	close(sd);

	// free used memory
	free(file_content);
	free(f_type);
	free(response);
	for(int i = 0; i < 3; ++i) {
		free(request_header[i]);
	}
}

char* build_response(char* status, char* type, char* file, struct tm* now) {
	
	char* response = malloc(50000);
	bzero(response, sizeof(response));

	char length[10];
	if(file != NULL) {
		sprintf(length, "%ld", strlen(file));
	}

	/* if error responses
	 * 400
	 * 404
	 * 405
	 */
	// bad request
	if(strcmp(status, "400") == 0) {
		strcpy(response, NOT_UNDERSTOOD);
		strcat(response, SERVER);
		strcat(response, DATE);
		strcat(response, asctime(now));
		strcat(response, "\n");
	}
	// file not found
	else if(strcmp(status, "404") == 0) {
		strcpy(response, FILE_NOT_FOUND);
		strcat(response, SERVER);
		strcat(response, DATE);
		strcat(response, asctime(now));
		strcat(response, TYPE);
		strcat(response, "text/html\r\n");
		strcat(response, LEN);
		strcat(response, NOT_FOUND_LENGTH);
		strcat(response, "\r\n\n");
		strcat(response, NOT_FOUND_BODY);
		strcat(response, "\n");
	}
	// method not allowed
	else if(strcmp(status, "405") == 0) {
		strcpy(response, METHOD_NOT_ALLOWED);
		strcat(response, SERVER);
		strcat(response, DATE);
		strcat(response, asctime(now));
        strcat(response, ALLOW);
		strcat(response, "\n");
	}
	// ok response
	else {
		strcpy(response, OK_RESPONSE);
		strcat(response, SERVER);
		strcat(response, DATE);
		strcat(response, asctime(now));
		strcat(response, TYPE);
		strcat(response, type);
        strcat(response, "\n");
		strcat(response, LEN);
		strcat(response, length);
		strcat(response, "\r\n\n");
		strcat(response, file);
		strcat(response, "\n");
	}

	return response;
}

void debug(char* ip, struct tm* now, char* line, char* status) {
	
	char* months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	
	printf("%s [%d/%s/%d %d:%d:%d] \"", ip, now->tm_mday, months[now->tm_mon], now->tm_year, now->tm_hour, now->tm_min, now->tm_sec, line, status);
}