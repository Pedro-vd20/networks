#ifndef STRINGS
#define STRINGS

/**
 * @brief parses first line of HTTP request and checks for errors
 *  
 * @param char* line of HTTP
 * @return char** [REQUEST TYPE, FILE REQUESTED, HTTP PROTOCOL] 
 */
char** parseHeader(char*);

/**
 * @brief Get the File Type of a file
 * 
 * @param string of file name
 * @return char* type of file (text, script, etc) 
 */
char* getFileType(char*);

#endif