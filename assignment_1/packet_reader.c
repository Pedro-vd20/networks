/*
 * Assignment 1
 * Reading UDP packets from PCAP files
 * Pedro Velasquez Duque
 * pv850
 * Due 13/02/2022
 */

#include <stdio.h>

int main(int argc, char** argv) {
    FILE* pcapPtr;
    // Check if file was passed and can be opened
    if(argc < 2 || (pcapPtr = fopen(argv[1], "r") == NULL)) {
        printf("File opening failed\n");
        return 1;
    }

    printf("YAY!\n");

    return 0;
}