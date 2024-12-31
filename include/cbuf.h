#ifndef CBUF_H__
#define CBUF_H__
#include <stddef.h>
#include "sentences.h"

size_t capacity(void);

void push(NMEA_SentenceType sentence);

void printall(void);

const NMEA_SentenceType * Front(void);

const NMEA_SentenceType * Back(void);

// CBUF_H__
#endif