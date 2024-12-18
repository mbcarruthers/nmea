#include <stdio.h>
#include <stdlib.h>
#include "nmea.h"

int main(void) {
    FILE* fp;
    fp = fopen("nmea.txt", "r");
    if(fp == NULL) {
        fprintf(stderr, "Error opening nmea.txt \n");
        return EXIT_FAILURE;
    }
    char buffer[256];

    while(fgets(buffer, sizeof(buffer), fp) != NULL) {
//          parse_sentence(buffer);
        push(parser(buffer));
    }
    printall();

    fclose(fp);
    return 0;
}