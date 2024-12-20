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
