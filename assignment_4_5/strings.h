#ifndef STRINGS
#define STRINGS

#include <string.h>
#include <stdlib.h>

/**
 * @brief get first line of multiline string
 * 
 * @param char* multiline string
 * @param char* array to store line
 * @return 0 if succesfull
 */
int get_first_line(char*, char*);

/**
 * @brief parses first line of HTTP request and checks for errors
 *  
 * @param char* [3] [REQUEST TYPE, FILE REQUESTED, HTTP PROTOCOL] 
 * @param char* entire request
 * @return 0 if successful, -1 if request not understood
 */
int parse_header(char* [3], char*);

/**
 * @brief Get the File Type of a file
 * 
 * @param string of file name
 * @return char* type of file (text, script, etc) 
 */
char* get_file_type(char*);

#endif