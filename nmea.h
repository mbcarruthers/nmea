#ifndef NMEA_H__
#define NMEA_H__

#include "cbuf.h"

#include "sentences.h"

#define TOKEN_SIZE 256
#define MAX_TOKENS 64
#define TOKEN_LENGTH 64

size_t parse_csv_line(const char * line, char tokens[MAX_TOKENS][TOKEN_LENGTH]);

NMEA_SentenceType parser(char * restrict str);

// parse_sentence original design for function
void parse_sentence(char * str);

// NMEA_H__
#endif