#include "sentences.h"

uint8_t nmea_to_mask(const char * restrict token) {
    switch (((token[2] & 0xFF) << 8) | (token[3] & 0xFF)) {
        case ('G' << 8) | 'G': return GPGGA;
        case ('G' << 8) | 'L': return GPGLL;
        case ('R' << 8) | 'M': return GPRMC;
        case ('V' << 8) | 'T': return GPVTG;
        case ('G' << 8) | 'S': return GPGSA;
        case ('G' << 8) | 'V': return GPGSV;
        case ('Z' << 8) | 'D': return GPZDA;
        case ('B' << 8) | 'S': return GPGBS;
        default: return 0;
    }
}