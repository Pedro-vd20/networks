/*
 * Assignment 1
 * Reading UDP packets from PCAP files
 * Pedro Velasquez Duque
 * pv850
 * Due 13/02/2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    FILE *pcapPtr;
    // Check if file was passed and can be opened

    if (argc < 2 || ((pcapPtr = fopen(argv[1], "rb")) == NULL))
    {
        printf("File opening failed\n");
        return 1;
    }

    // global constants for file format
    const int G_HEADER = 24, R_HEADER = 16, E_HEADER = 14, IP_HEADER = 20,
              NUM_PACKETS = 5, HEADER_ITEMS = 4;
    const char const *DIVIDER = "-------------------------";

    // skip global header
    fseek(pcapPtr, G_HEADER, SEEK_SET);

    printf("%s\n", DIVIDER);

    for (int i = 0; i < NUM_PACKETS; ++i)
    {
        // skip header info
        fseek(pcapPtr, R_HEADER + E_HEADER + IP_HEADER, SEEK_CUR);

        // collect header info
        unsigned short header[HEADER_ITEMS];
        fread(header, sizeof(short), HEADER_ITEMS, pcapPtr);

        // print header data
        printf("Src Port: %hu\n", ntohs(header[0]));
        printf("Des Port: %hu\n", ntohs(header[1]));
        printf("UDP Packet Length: %hu\n", ntohs(header[2]));
        printf("Checksum: 0x%x\n", ntohs(header[3]));

        // collect data
        int dataSize = (int)ntohs(header[2]) - 8;
        char* data = malloc(dataSize + 1);
        data[dataSize] = '\0';

        fread(data, dataSize, 1, pcapPtr);

        // print data
        // printf("%s\n", data);
        // Must print like this since there can be '\0' inside the data
        for(int i = 0; i < dataSize; ++i) {
            printf("%c", data[i]);
        }
        printf("\n");

        free(data);
        printf("%s\n", DIVIDER);
    }

    fclose(pcapPtr);

    return 0;
}