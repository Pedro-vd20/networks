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
    
    
    if(argc < 2 || ((pcapPtr = fopen(argv[1], "rb")) == NULL)) {
        printf("File opening failed\n");
        return 1;
    }

    // global constants for file format
    const int G_HEADER = 24, R_HEADER = 16, E_HEADER = 14, IP_HEADER = 20;
    const int UDP_HEADER = 4;
    const int NUM_PACKETS = 2;
    const char const * DIVIDER = "-------------------------";

    // skip global header
    fseek(pcapPtr, G_HEADER, SEEK_SET);

    printf("%s\n", DIVIDER);

    short srcPort, destPort, length, checkSum;

    /*// loop through both packets
    for(int i = 0; i < 1; ++i) {
        unsigned short srcPort, destPort, length, checkSum;

        // skip headers
        fseek(pcapPtr, R_HEADER + E_HEADER + IP_HEADER, SEEK_CUR);

        // read UDP header
        fscanf(pcapPtr, "%hu%hu%hu%hu", &srcPort, &destPort, &length, &checkSum);

        // convert to host byte order
        srcPort = ntohs(srcPort);
        destPort = ntohs(destPort);
        length = ntohs(length);
        checkSum = ntohs(checkSum);

        // print file info
        printf("Src Port: %hu\nDes Port: %hu\nUDP Packet Length: %hu\nChecksum: %x\n", srcPort, destPort, length, checkSum);
    }*/

    printf("YAY!\n");

    return 0;
}