#include <stdio.h>

#include "strings.h"

int get_first_line(char* request, char* line) {
	
	// find index of newline
	char* ptr = strstr(request, "\n");

	bzero(line, sizeof(line));
	strncpy(line, request, ptr - request);

    return 0;
}

int parse_header(char* header[3], char* line) {

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

char* get_file_type(char* file) {
    /* Expected file types:
     * text/html
     * application/javascript
     * text/css
     * image/png
     */

    // creat temp stringstrlen(file) (strtok doesn't work on parameter)
    int len = strlen(file);
    char* f = malloc(len + 1);
    strcpy(f, file);
    f[len] = '\0';

    // parse file by file_name + file_type
    char* token = strtok(f, ".");
    if(token) { // not NULL
        token = strtok(NULL, ".");
    }

    if(!token) {
        free(f);
        return NULL;
    }

    char* file_type = malloc(50);
    bzero(file_type, sizeof(file_type));
    if(strcmp(token, "html") == 0) {
        strcpy(file_type, "text/html");
    }
    else if(strcmp(token, "css") == 0) {
        strcpy(file_type, "text/css");
    }
    else if(strcmp(token, "js") == 0) {
        strcpy(file_type, "application/javascript");
    }
    else if(strcmp(token, "png") == 0) {
        strcpy(file_type, "image/png");
    }
    else {
        free(f);
        free(file_type);
        return NULL;
    }

    free(f);
    return file_type; 
}