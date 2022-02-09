#include <stdio.h>

// structures group data like a class
// can't hold functions, only data
struct student {
    char id[9];
    char name[26];
    float gpa;
};

// declaring nameless structure to initialize only 1 element
struct {
    char id[9];
    char name[26];
    float gpa;
} st2;

// using typedef to define struct
// puts name at the end
typedef struct {
    char id[9];
    char name[26];
    float gpa;
} STUDENT;

int main() {
    struct student st1 = {"ak100", "Ali Khan", 3.5};    // must write struct at the beginning
    STUDENT st3 = {"pv850", "Pedro Velasquez", 3.9};    // no need to write struct at the beginning 

    // opening files require file pointers (struct defined on standard library)
    FILE* ptr;
    ptr = fopen("text.txt", "w");   // open in write mode
    // other modes include write (w), append (a), binary read (rb), binary writing (wb)
    // modes can be appended using +

    // if the file couldn't be openend, the pointer will be NULL
    FILE* fptr;
    if ((fptr = fopen("reading.txt", "r")) == NULL) {
        perror("File opening failed");
        return 1;
    }

    // characters
    // retrieving single character
    while(1) {
        char ch;
        ch = fgetc(fptr);
        if (ch == EOF) { //check end of file
            printf("\n");
            break;
        }
        printf("%c", ch);
    }

    // getting string
    // fgets(char* buffer, int size, FILE* fstream); can specify max characters to read (avoiding injection vulnerabilities)

    // scanning ints and floats
    // fscan(FILE* in_stream, "%i, %c %f", &i, &ch, &f);

    fclose(fptr);
    fclose(ptr);

    return 0;
}