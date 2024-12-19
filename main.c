#include <stdio.h>
#include <stdlib.h>
#include "nmea.h"

// basic operations
// open file
// read file by line
// parse line
// push to array
// print array

int main(void) {
    FILE* fp;
    fp = fopen("nmea.txt", "r");
    if(fp == NULL) {
        fprintf(stderr, "Error opening nmea.txt \n");
        return EXIT_FAILURE;
    }
    char buffer[256];

    while(fgets(buffer, sizeof(buffer), fp) != NULL) {
        push(parser(buffer));
    }
    printall();
    fclose(fp);
    return EXIT_SUCCESS;
}