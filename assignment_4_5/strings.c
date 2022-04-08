#include "strings.h"

int parse_header(char* header[3], char* line, int length) {

    char* str = malloc(length + 1);
    memcpy(str, line, length);
    str[length] = '\0';

    // collect first line
    char* first_line = strtok(str, "\n");
    if(first_line == NULL) {
        free(str);
        return -1; // wrong format
    } 
    strcat(first_line, " ");

    
    // Extract the first word
    char * token = strtok(first_line, " ");
    int len = strlen(token);
    header[0] = malloc(len + 1);
    header[0][len] = '\0';
    strncpy(header[0], token, strlen(token));
    
    // loop through the string to extract all other words
    for(int i = 1; i < 3; ++i) {
        token = strtok(NULL, " ");
        // if error reading word
        if(token == NULL) { 
            free(str);
            return -1;
        }

        len = strlen(token);
        header[i] = malloc(len + 1);
        strncpy(header[i], token, len);
        header[i][len] = '\0';
    }
    
    
    free(str);
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