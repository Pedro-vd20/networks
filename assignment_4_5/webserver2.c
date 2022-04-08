#include<stdio.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<string.h>

int main()
{

	//1. socket
	int server_sd = socket(AF_INET,SOCK_STREAM,0);
	if(server_sd<0)
	{
		perror("Socket Failed: ");
		return -1;
	}

	//2. bind
	struct sockaddr_in server_address;	//structure to save IP address and port
	memset(&server_address,0,sizeof(server_address)); //Initialize/fill the server_address to 0
	server_address.sin_family = AF_INET;	//address family
	server_address.sin_port = htons(8080);	//port
	server_address.sin_addr.s_addr = htonl(INADDR_ANY); //htonl(INADDR_LOOPBACK); //inet_addr("127.0.0.1");


	if(bind(server_sd,(struct sockaddr *)&server_address,sizeof(server_address))<0) 
	{
		perror("Bind failed..:");
		return -1;
	}

	//3. listen
	if(listen(server_sd,5)<0)
	{
		perror("Listen Error:");
		return -1;
	}


	struct sockaddr_in client_address;  		//to store the client information

	//4. accept
	while(1)
	{
		//int client_sd = accept(server_sd,NULL,NULL);]
		int client_len=sizeof(client_address);	//lent of client cliend address of type sockaddr_in
		int client_sd = accept(server_sd,(struct sockaddr*)&client_address,(socklen_t *)&client_len);	//accept the connection but also fill the client address with client info
		if(client_sd<1)
		{
			perror("Accept Error:");
			return -1;
		}

		char client_name[100];		//to store client address 
		inet_ntop(AF_INET,&client_address.sin_addr,client_name,sizeof(client_name));
		//The reason why we were getting wrong client addresses at the end of todays session because 
		//instead of converting client_address.sin_addr to string were were converting client_address. 

		printf("Connection established from %s \n",client_name);

		char response[1000]="HTTP/1.1 200 OK \r\n\n";		//http header

		FILE* file = fopen("index.html","r");				//open the index.html file
		if(file == NULL)
		{
			perror("File error:");
			return -1;
		}
		char line[256];
		while(fgets(line,sizeof(line),file)!=0)				//concatinate the index.html file line by line into server response
		{
			strcat(response,line);
		}
		fclose(file);
		
		//5. send
		send(client_sd,response,sizeof(response),0);		//send the server response to the client
		close(client_sd);									//close the client socket
	}	

	close(server_sd);										//close the server socket
	//6. close


}
