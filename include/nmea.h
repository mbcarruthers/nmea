#ifndef NMEA_H__
#define NMEA_H__

#include "cbuf.h"
#include "sentences.h"

#define TOKEN_SIZE 256
#define MAX_TOKENS 64
#define TOKEN_LENGTH 64

typedef struct {
    const char* token;
    NMEA_Mask mask;
} tokenMap;

size_t parse_csv_line(const char * line, char tokens[MAX_TOKENS][TOKEN_LENGTH]);

NMEA_Mask filter_mask(const char * token);

NMEA_Mask nmea_to_mask(const char * token);

NMEA_SentenceType parser(char * restrict str);


// NMEA_H__
#endif