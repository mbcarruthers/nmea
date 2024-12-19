#include "sentences.h"
#include <string.h>
// bit twiddling hacks - Stanford
// it works don't touch it
// returns what type of

typedef struct {
    const char* token;
    NMEA_Mask mask;
} tokenMap;

static const tokenMap nmea_map[] = {
        {"GPGGA", GPGGA },
        {"GPGLL", GPGLL },
        {"GPRMC", GPRMC },
        {"GPVTG", GPVTG },
        {"GPGSA", GPGSA },
        {"GPGSV", GPGSV },
        {"GPZDA", GPZDA },
        {"GPGBS", GPGBS }
};

NMEA_Mask nmea_to_mask(const char *token) {
    for (int i = 0; nmea_map[i].token != NULL; i++) {
        if (strcmp(token, nmea_map[i].token) == 0) {
            return nmea_map[i].mask;
        }
    }
    return UNKNOWN;
}

// nmea_to_mask first iteration 1st iteration
// Tried to be bit hacky - ran into unknown error
//uint8_t nmea_to_mask(const char * token) {
//    switch (((token[2] & 0xFF) << 8) | (token[3] & 0xFF)) {
//        case ('G' << 8) | 'G': return GPGGA;
//        case ('G' << 8) | 'L': return GPGLL;
//        case ('R' << 8) | 'M': return GPRMC;
//        case ('V' << 8) | 'T': return GPVTG;
//        case ('G' << 8) | 'S': return GPGSA;
//        case ('G' << 8) | 'V': return GPGSV;
//        case ('Z' << 8) | 'D': return GPZDA;
//        case ('B' << 8) | 'S': return GPGBS;
//        default: return 0;
//    }
//}