#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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

char* build_response(char* status, char* type, char* file) {
	
	char* response = malloc(50000);

	// collect date
	time_t now = time(NULL);
	struct tm* time = localtime(&now);

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
		strcat(response, NOT_UNDERSTOOD);
		strcat(response, SERVER);
		strcat(response, DATE);
		strcat(response, asctime(time));
		strcat(response, "\n");
	}
	// file not found
	else if(strcmp(status, "404") == 0) {
		strcat(response, FILE_NOT_FOUND);
		strcat(response, SERVER);
		strcat(response, DATE);
		strcat(response, asctime(time));
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
		strcat(response, METHOD_NOT_ALLOWED);
		strcat(response, SERVER);
		strcat(response, DATE);
		strcat(response, asctime(time));
        strcat(response, ALLOW);
		strcat(response, "\n");
	}
	// ok response
	else {
		strcat(response, OK_RESPONSE);
		strcat(response, SERVER);
		strcat(response, DATE);
		strcat(response, asctime(time));
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

int main() {

    char* status = "200";
    char* type = "application/javascript";
    char* file = NULL;

    char* response = build_response(status, type, (char*)NOT_FOUND_BODY);

    printf("%s\n", response);

    return 0;
}
