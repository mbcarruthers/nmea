#include <stdio.h>
#include <stdlib.h>
#include "nmea.h"
#include "console.h"

// basic operations
// open file
// read file by line
// parse line
// push to array
// print array



int main(int argc, char * argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file path>\n", argv[0]);
        return 1;
    }
    FILE* fp;
    fp = fopen(argv[1], "r");
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