#ifndef CBUF_H__
#define CBUF_H__

#include <stddef.h>
#include "sentences.h"

size_t capacity(void);

void push(NMEA_SentenceType sentence);
void printall(void);
// CBUF_H__
#endif