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


//// Note: Original tightly coupled printing mechanism
//void printall__(void) {
//    for(size_t i = 0; i < capacity();i++) {
//       switch(recvd[i].nmea) {
//           case GPGGA:
//               print_GPGGA_Sentence(&recvd[i].value.gpgga);
//               break;
//           case GPGLL:
//               print_GPGLL_Sentence(&recvd[i].value.gpgll);
//               break;
//           case GPRMC:
//               print_GPRMC_Sentence(&recvd[i].value.gprmc);
//               break;
//           case GPVTG:
//               print_GPVTG_Sentence(&recvd[i].value.gpvtg);
//               break;
//           case GPGSA:
//               print_GPGSA_Sentence(&recvd[i].value.gpgsa);
//               break;
//           case GPGSV:
//               print_GPGSV_Sentence(&recvd[i].value.gpgsv);
//               break;
//           case GPZDA:
//               print_GPZDA_Sentence(&recvd[i].value.gpzda);
//               break;
//           case GPGBS:
//               print_GPGBS_Sentence(&recvd[i].value.gpgbs);
//               break;
//           case UNKNOWN:
//               printf("Unknown printall called\n");
//               break;
//            default:
//               printf("default printall called \n");
//               break;
//       }
//    }
//}
