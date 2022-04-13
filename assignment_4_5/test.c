#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int parse_header(char* header[3], char* line, int length) {

	// collect GET
	header[0] = malloc(10);
	bzero(header[0], 10);

	char* ptr = strstr(line, " ");
	strncpy(header[0], line, ptr - line);

	// get file Name
	header[1] = malloc(50);
	bzero(header[1], 50);
	
	ptr = strstr(line + 4, " ");
	strncpy(header[1], line + 4, ptr-line-4);

	// get HTTP
	header[2] = malloc(10);
	bzero(header[2], 10);
	strcpy(header[2], ptr + 1);
	
    return 0;

}


int main() {
	char str[100] = "GET /index.html HTTP/1.1";
	char* l[3];

	parse_header(l, str, 0);

	printf("%s\n", l[1]);

	return 0;
}