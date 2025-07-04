#include "nmea.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// Todo:
// Token length and max tokens should be able to be greatly reduced
// from their original inflated sizes
// Max_tokens ~12-24
// token length ~8-12

// ========================= Parsing functions =========================================

size_t parse_csv_line(const char * line, char tokens[MAX_TOKENS][TOKEN_LENGTH]) {
    if(line[0] != '$') {
        return 0;
    }
    size_t token_count = 0;
    const char *curr = line;

    while ((*curr != '\0') && (*curr != '\n') && token_count < MAX_TOKENS) {

        size_t length = strcspn(curr, ",");

        if (length >= TOKEN_LENGTH) {
            fprintf(stderr, "Token exceeds maximum size.\n");
            return token_count;
        }
        memcpy(tokens[token_count], curr, length);
        tokens[token_count][length] = '\0';

        token_count++;
        curr += length;

        if (*curr == ',') {
            curr++;
        }
    }

    return token_count;

}

static inline float parse_float(const char * token) {
    return token ? strtof(token, NULL) : 0.0f;
}

static inline uint32_t parse_uint32(const char * token) {
    return token ? (uint32_t) strtoul(token, NULL, 10) : 0;
}

NMEA_Mask filter_mask(const char * token) {
    if (strncmp(token, "GPGGA", 5) == 0) return GPGGA;
    if (strncmp(token, "GPGLL", 5) == 0) return GPGLL;
    if (strncmp(token, "GPRMC", 5) == 0) return GPRMC;
    if (strncmp(token, "GPVTG", 5) == 0) return GPVTG;
    if (strncmp(token, "GPGSA", 5) == 0) return GPGSA;
    if (strncmp(token, "GPGSV", 5) == 0) return GPGSV;
    if (strncmp(token, "GPZDA", 5) == 0) return GPZDA;
    if (strncmp(token, "GPGBS", 5) == 0) return GPGBS;
    return UNKNOWN;
}

// ========================== Data Handling =============================================

static inline void handle_GPGGA_Sentence(NMEA_SentenceType * sentence,
                                         char tokens[MAX_TOKENS][TOKEN_LENGTH]) {
    sentence->value.gpgga.utc_time = parse_uint32(tokens[1]);
    sentence->value.gpgga.latitude = parse_float(tokens[2]);
    sentence->value.gpgga.lat_dir = (char)(*tokens[3]);
    sentence->value.gpgga.longitude = parse_float(tokens[4]);
    sentence->value.gpgga.lon_dir = (char)(*tokens[5]);
    sentence->value.gpgga.fix_quality = (uint8_t)atoi(tokens[6]);
    sentence->value.gpgga.num_satellites = (uint8_t)atoi(tokens[7]);
    sentence->value.gpgga.hdop = parse_float(tokens[8]);
    sentence->value.gpgga.altitude = parse_float(tokens[9]);
    sentence->value.gpgga.geoidal_seperation = parse_float(tokens[11]);
}

static inline void handle_GPGLL_Sentence(NMEA_SentenceType * sentence,
                                         char tokens[MAX_TOKENS][TOKEN_LENGTH]) {
    sentence->value.gpgll.latitude = parse_float(tokens[1]);
    sentence->value.gpgll.lat_dir = (char)(*tokens[2]);
    sentence->value.gpgll.longitude = parse_float(tokens[3]);
    sentence->value.gpgll.lon_dir = (char)(*tokens[4]);
    sentence->value.gpgll.utc_time = parse_uint32(tokens[5]);
    sentence->value.gpgll.status = (char)(*tokens[6]);
}

static inline void handle_GPRMC_Sentence(NMEA_SentenceType * sentence,
                                         char tokens[MAX_TOKENS][TOKEN_LENGTH]) {
    sentence->value.gprmc.utc_time = parse_uint32(tokens[1]);
    sentence->value.gprmc.status = (char)(*tokens[2]);
    sentence->value.gprmc.latitude = parse_float(tokens[3]);
    sentence->value.gprmc.lat_dir = (char)(*tokens[4]);
    sentence->value.gprmc.longitude = parse_float(tokens[5]);
    sentence->value.gprmc.lon_dir = (char)(*tokens[6]);
    sentence->value.gprmc.speed_over_ground = parse_float(tokens[7]);
    sentence->value.gprmc.course_over_ground = parse_float(tokens[8]);
    sentence->value.gprmc.date = parse_uint32(tokens[9]);
    sentence->value.gprmc.magnetic_variation = parse_float(tokens[10]);
    sentence->value.gprmc.var_dir = (char)(*tokens[11]);
}

static inline void handle_GPVTG_Sentence(NMEA_SentenceType * sentence,
                                         char tokens[MAX_TOKENS][TOKEN_LENGTH]) {
    sentence->value.gpvtg.true_track = parse_float(tokens[1]);
    sentence->value.gpvtg.true_indicator = (char)(*tokens[2]);
    sentence->value.gpvtg.magnetic_track = parse_float(tokens[3]);
    sentence->value.gpvtg.magnetic_indicator = (char)(*tokens[4]);
    sentence->value.gpvtg.speed_knots = parse_float(tokens[5]);
    sentence->value.gpvtg.knots_indicator = (char)(*tokens[6]);
    sentence->value.gpvtg.speed_kmph = parse_float(tokens[7]);
    sentence->value.gpvtg.kmph_indicator = (char)(*tokens[8]);
}

static inline void handle_GPGSA_Sentence(NMEA_SentenceType * sentence, size_t count,
                                         char tokens[MAX_TOKENS][TOKEN_LENGTH]) {
    sentence->value.gpgsa.mode = tokens[1][0]; // 'A' or 'M'
    sentence->value.gpgsa.fix_type = (uint8_t)atoi(tokens[2]);

    // parse 12 satellites
    for (int i = 0; i < 12; i++) {
        if (tokens[3 + i][0] == '\0' || !isdigit((unsigned char)tokens[3 + i][0])) {
            sentence->value.gpgsa.satellites[i] = 0;
        } else {
            sentence->value.gpgsa.satellites[i] = (uint8_t)atoi(tokens[3 + i]);
        }
    }


    float pdop = 0.0f, hdop = 0.0f, vdop = 0.0f;
    int found = 0; // how many numeric fields found from the end

    // count is the number of tokens returned by parse_csv_line
    // find diluation variables for GPGSA by iterating backwards
    for (int i = (int)count - 1; (i >= 0) && (found < 3); i--) {
        if (tokens[i][0] != '\0') {

            float val = parse_float(tokens[i]);
            // if its a float it's our value
            if (found == 0) {
                vdop = val;
            } else if (found == 1) {
                hdop = val;
            } else if (found == 2) {
                pdop = val;
            }
            found++;
        }
    }

    // If fewer than 3 fields found, missing remain 0.0 by default
    sentence->value.gpgsa.pdop = pdop;
    sentence->value.gpgsa.hdop = hdop;
    sentence->value.gpgsa.vdop = vdop;
}

static inline void handle_GPGSV_Sentence(NMEA_SentenceType * sentence,
                                         char tokens[MAX_TOKENS][TOKEN_LENGTH]) {
    sentence->value.gpgsv.total_messages = (uint8_t)atoi(tokens[1]);
    sentence->value.gpgsv.message_number = (uint8_t)atoi(tokens[2]);
    sentence->value.gpgsv.satellites_in_view = (uint8_t)atoi(tokens[3]);
    // base = offset used to calculate the index of satellite data tokens
    for (int i = 0; i < 4; i++) {
        int base = 4 + (i * 4); // * 4 accounts for the four attributes
        // Note: if empty data then satellites info zero initialized
        if (tokens[base][0] == '\0') {
            sentence->value.gpgsv.satellite_info[i].ptn = 0;
            sentence->value.gpgsv.satellite_info[i].elevation = 0;
            sentence->value.gpgsv.satellite_info[i].azimuth = 0;
            sentence->value.gpgsv.satellite_info[i].snr = 0;
        } else {
            sentence->value.gpgsv.satellite_info[i].ptn = parse_uint32(tokens[base]);
            sentence->value.gpgsv.satellite_info[i].elevation = parse_uint32(tokens[base + 1]);
            sentence->value.gpgsv.satellite_info[i].azimuth = parse_uint32(tokens[base + 2]);
            sentence->value.gpgsv.satellite_info[i].snr = (uint8_t)atoi(tokens[base + 3]);
        }
    }
}

static inline void handle_GPZDA_Sentence(NMEA_SentenceType * sentence,
                                         char tokens[MAX_TOKENS][TOKEN_LENGTH]) {
    sentence->value.gpzda.utc_time = parse_uint32(tokens[1]);
    sentence->value.gpzda.day = (uint8_t)atoi(tokens[2]);
    sentence->value.gpzda.month = (uint8_t)atoi(tokens[3]);
    sentence->value.gpzda.year = (uint16_t)atoi(tokens[4]);
    sentence->value.gpzda.local_hour_offset = (uint8_t)atoi(tokens[5]);
    sentence->value.gpzda.local_minute_offset = (uint8_t)atoi(tokens[6]);
}

static inline void handle_GPGBS_Sentence(NMEA_SentenceType * sentence,
                                         char tokens[MAX_TOKENS][TOKEN_LENGTH]) {
    sentence->value.gpgbs.utc_time = parse_uint32(tokens[1]);
    sentence->value.gpgbs.horizontal_error = parse_float(tokens[2]);
    sentence->value.gpgbs.vertical_error = parse_float(tokens[3]);
    sentence->value.gpgbs.position_error = parse_float(tokens[4]);
}
// =======================================================================================

// parser - the BIG function.
// note: check for error by checking for unknown
NMEA_SentenceType parser(char * restrict str) {
    char tokens[MAX_TOKENS][TOKEN_LENGTH];
    NMEA_SentenceType sentence = {0};
    const size_t count = parse_csv_line(str, tokens);
    if(!count) {
        sentence.nmea = UNKNOWN;
        return sentence;
    }

    char * sentence_type = tokens[0] + sizeof(char); // string variable for NMEA sentence type - sizeof(char) for explicitness

    sentence.nmea = filter_mask(sentence_type);
    switch(sentence.nmea) {
        case GPGGA:
            handle_GPGGA_Sentence(&sentence, tokens);
            return sentence;
            break;
        case GPGLL:
            handle_GPGLL_Sentence(&sentence, tokens);
            return sentence;
            break;
        case GPRMC:
            handle_GPRMC_Sentence(&sentence, tokens);
            return sentence;
            break;
        case GPVTG:
            handle_GPVTG_Sentence(&sentence, tokens);
            return sentence;
            break;
        case GPGSA:
            handle_GPGSA_Sentence(&sentence, count, tokens);
            return sentence;
            break;
        case GPGSV:
            handle_GPGSV_Sentence(&sentence, tokens);
            return sentence;
            break;
        case GPZDA:
            handle_GPZDA_Sentence(&sentence, tokens);
            return sentence;
            break;
        case GPGBS:
            handle_GPGBS_Sentence(&sentence, tokens);
            return sentence;
            break;
        case UNKNOWN:
            printf("Unknown detected \n");
            return sentence;  // returns empty unknown
            break;
        default:
            sentence.nmea = UNKNOWN;
            return sentence; // returnsempty unknown
            break;
    }
    return sentence;
}


// TODO: Consider using lookup tables, already have bitmasks ready, for parser