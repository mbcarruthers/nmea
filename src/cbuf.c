#include <stdio.h>
#include <stdlib.h>
#include "cbuf.h"
// Circular buffer
#define BUFFER_LENGTH 24
static NMEA_SentenceType recvd[BUFFER_LENGTH];
static NMEA_SentenceType * front = recvd;
static NMEA_SentenceType * const back = recvd + BUFFER_LENGTH;
static size_t count = 0; // Not really necessary

// capacity hold where the array is currently at
size_t capacity(void) {
    return (size_t)(front >= back) ? (front - back) : ((sizeof(recvd) / sizeof(recvd[0])) - (back- front));
}

void push(NMEA_SentenceType sentence) {
    if(sentence.nmea == UNKNOWN) return;

    *front = sentence;
    if (count < BUFFER_LENGTH) {
        count++;
    } else {
        // if full overwrite old data, count stays at BUFFER_LENGTH
    }
    front++;
    if (front == back) {
        front = recvd;
    }
}

// read only access - should check for NULL 
const NMEA_SentenceType * Front(void) {
    return recvd[0].nmea ? &recvd[0] : NULL;
}

const NMEA_SentenceType * Back(void) {
    return &recvd[capacity()];
}



