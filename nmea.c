#include "nmea.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// ------------------------ Parsing functions ---------------------------------------
size_t parse_csv_line(const char * line, char tokens[MAX_TOKENS][TOKEN_LENGTH]) {
    size_t token_count = 0;
    const char *cur = line;

    while (*cur != '\0' && token_count < MAX_TOKENS) {
        // Find the length of the next token by scanning until a comma or end of string
        size_t length = strcspn(cur, ",");

        if (length >= TOKEN_LENGTH) {
            fprintf(stderr, "Token exceeds maximum size.\n");
            // Return what we got so far; no partial token stored
            return token_count;
        }

        // Copy exactly 'length' chars into the token
        memcpy(tokens[token_count], cur, length);
        tokens[token_count][length] = '\0'; // Null-terminate

        token_count++;

        // Advance past this token
        cur += length;

        // If we're at a comma, skip it to move to the next token
        if (*cur == ',') {
            cur++;
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

// ---------------------- Data Handling -----------------------------------------------

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

static inline void handle_GPGLL_Sentence(struct GPGLL_Sentence * s,
        char tokens[MAX_TOKENS][TOKEN_LENGTH]) {
    // Note: Implement the rest
}

static inline void handle_GPRMC_Sentence(struct GPRMC_Sentence * s,
        char tokens[MAX_TOKENS][TOKEN_LENGTH]) {
    // Note: implement the rest
}

// parser - the BIG function.
// TODO: Consider using lookup tables, already have bitmasks ready
NMEA_SentenceType parser(char * restrict str) {
    char tokens[MAX_TOKENS][TOKEN_LENGTH];
    // Note: unused and known
    const size_t count = parse_csv_line(str, tokens);

    // now all values are comma seperated
    NMEA_SentenceType sentence = {0};
    char * sentence_type = tokens[0] + sizeof(char); // string variable for NMEA sentence type - sizeof(char) for explicitness

    sentence.nmea = nmea_to_mask(sentence_type);
    // Todo: declare iterator(not i) = 0 in switch,i++ indexs.


    switch(sentence.nmea) {
        case GPGGA:
//            sentence.value.gpgga.utc_time = parse_uint32(tokens[1]);
//            sentence.value.gpgga.latitude = parse_float(tokens[2]);
//            sentence.value.gpgga.lat_dir = (char)(*tokens[3]);
//            sentence.value.gpgga.longitude = parse_float(tokens[4]);
//            sentence.value.gpgga.lon_dir = (char)(*tokens[5]);
//            sentence.value.gpgga.fix_quality = (uint8_t)(*tokens[6]);
//            sentence.value.gpgga.num_satellites = (uint8_t)(*tokens[7]);
//            sentence.value.gpgga.hdop = parse_float(tokens[8]);
//            sentence.value.gpgga.altitude = parse_float(tokens[9]);
//            sentence.value.gpgga.geoidal_seperation = parse_float(tokens[11]);
            handle_GPGGA_Sentence(&sentence, tokens);
            return sentence;
            break;
        case GPGLL:
            sentence.value.gpgll.latitude = parse_float(tokens[1]);
            sentence.value.gpgll.lat_dir = (char)(*tokens[2]);
            sentence.value.gpgll.longitude = parse_float(tokens[3]);
            sentence.value.gpgll.lon_dir = (char)(*tokens[4]);
            sentence.value.gpgll.utc_time = parse_uint32(tokens[5]);
            sentence.value.gpgll.status = (char)(*tokens[6]);
            return sentence;
            break;
        case GPRMC:
            sentence.value.gprmc.utc_time = parse_uint32(tokens[1]);
            sentence.value.gprmc.status = (char)(*tokens[2]);
            sentence.value.gprmc.latitude = parse_float(tokens[3]);
            sentence.value.gprmc.lat_dir = (char)(*tokens[4]);
            sentence.value.gprmc.longitude = parse_float(tokens[5]);
            sentence.value.gprmc.lon_dir = (char)(*tokens[6]);
            sentence.value.gprmc.speed_over_ground = parse_float(tokens[7]);
            sentence.value.gprmc.course_over_ground = parse_float(tokens[8]);
            sentence.value.gprmc.date = parse_uint32(tokens[9]);
            sentence.value.gprmc.magnetic_variation = parse_float(tokens[10]);
            sentence.value.gprmc.var_dir = (char)(*tokens[11]);
            return sentence;
            break;
        case GPVTG:
            sentence.value.gpvtg.true_track = parse_float(tokens[1]);
            sentence.value.gpvtg.true_indicator = (char)(*tokens[2]);
            sentence.value.gpvtg.magnetic_track = parse_float(tokens[3]);
            sentence.value.gpvtg.magnetic_indicator = (char)(*tokens[4]);
            sentence.value.gpvtg.speed_knots = parse_float(tokens[5]);
            sentence.value.gpvtg.knots_indicator = (char)(*tokens[6]);
            sentence.value.gpvtg.speed_kmph = parse_float(tokens[7]);
            sentence.value.gpvtg.kmph_indicator = (char)(*tokens[8]);
            return sentence;
            break;
        case GPGSA:
            sentence.value.gpgsa.mode = tokens[1][0]; // 'A' or 'M'
            sentence.value.gpgsa.fix_type = (uint8_t)atoi(tokens[2]);

            // Parse satellites (up to 12, empty fields = 0)
            for (int i = 0; i < 12; i++) {
                if (tokens[3 + i][0] == '\0' || !isdigit((unsigned char)tokens[3 + i][0])) {
                    sentence.value.gpgsa.satellites[i] = 0;
                } else {
                    sentence.value.gpgsa.satellites[i] = (uint8_t)atoi(tokens[3 + i]);
                }
            }

            // Identify PDOP, HDOP, VDOP by scanning from the end:
            float pdop = 0.0f, hdop = 0.0f, vdop = 0.0f;
            int found = 0; // how many numeric fields found from the end

            // count is the number of tokens returned by parse_csv_line
            for (int i = (int)count - 1; i >= 0 && found < 3; i--) {
                if (tokens[i][0] != '\0') {
                    // Attempt to parse as float
                    float val = parse_float(tokens[i]);

                    // Assign values in reverse order
                    if (found == 0) {
                        vdop = val; // First numeric from end
                    } else if (found == 1) {
                        hdop = val; // Second numeric from end
                    } else if (found == 2) {
                        pdop = val; // Third numeric from end
                    }
                    found++;
                }
            }

            // If fewer than 3 fields found, missing remain 0.0 by default
            sentence.value.gpgsa.pdop = pdop;
            sentence.value.gpgsa.hdop = hdop;
            sentence.value.gpgsa.vdop = vdop;

            return sentence;
            break;
        case GPGSV:
            sentence.value.gpgsv.total_messages = (uint8_t)atoi(tokens[1]);
            sentence.value.gpgsv.message_number = (uint8_t)atoi(tokens[2]);
            sentence.value.gpgsv.satellites_in_view = (uint8_t)atoi(tokens[3]);
            // base = offset used to calculate the index of satellite data tokens
            for (int i = 0; i < 4; i++) {
                int base = 4 + (i * 4); // * 4 accounts for the four attributes
                // Note: if empty data then satellites info zero initialized
                if (tokens[base][0] == '\0') {
                    sentence.value.gpgsv.satellite_info[i].ptn = 0;
                    sentence.value.gpgsv.satellite_info[i].elevation = 0;
                    sentence.value.gpgsv.satellite_info[i].azimuth = 0;
                    sentence.value.gpgsv.satellite_info[i].snr = 0;
                } else {
                    sentence.value.gpgsv.satellite_info[i].ptn = parse_uint32(tokens[base]);
                    sentence.value.gpgsv.satellite_info[i].elevation = parse_uint32(tokens[base + 1]);
                    sentence.value.gpgsv.satellite_info[i].azimuth = parse_uint32(tokens[base + 2]);
                    sentence.value.gpgsv.satellite_info[i].snr = (uint8_t)atoi(tokens[base + 3]);
                }
            }
            return sentence;
            break;
        case GPZDA:
            sentence.value.gpzda.utc_time = parse_uint32(tokens[1]);
            sentence.value.gpzda.day = (uint8_t)atoi(tokens[2]);
            sentence.value.gpzda.month = (uint8_t)atoi(tokens[3]);
            sentence.value.gpzda.year = (uint16_t)atoi(tokens[4]);
            sentence.value.gpzda.local_hour_offset = (uint8_t)atoi(tokens[5]);
            sentence.value.gpzda.local_minute_offset = (uint8_t)atoi(tokens[6]);
            return sentence;
            break;
        case GPGBS:
            sentence.value.gpgbs.utc_time = parse_uint32(tokens[1]);
            sentence.value.gpgbs.horizontal_error = parse_float(tokens[2]);
            sentence.value.gpgbs.vertical_error = parse_float(tokens[3]);
            sentence.value.gpgbs.position_error = parse_float(tokens[4]);
            return sentence;
            break;
        case UNKNOWN:
            printf("Unknown detected \n");
            return sentence;  // returns empty sentence
            break;
        default:
            printf("------------------------Default ------------------\n");
            return sentence; // returns empty sentence
            break;
    }
    return sentence;
}

// todo remove?
// parse_sentence - unused currently. Keep for now. 12.18.24
void parse_sentence(char * str) {
    char token[TOKEN_SIZE] = {0}, *strp = str;
    char tokens[MAX_TOKENS][TOKEN_LENGTH];
//    struct NMEA_SentenceType *sentence;
    size_t count = parse_csv_line(str, tokens);
    printf("Count of csv file %zu \n",
           count); // to get compiler off my back abt an unused fn


    while (*strp) {
        if (*strp == '$') {
            strp++;
            int i = 0;
            while (*strp != ',' && *strp != '\0') {
                token[i++] = *strp++; // copy char move pointer. beautiful
            }
            token[i] = '\0';
            NMEA_Mask mask = nmea_to_mask(token);
            printf("Token %s \n Mask %x \n",
                   token, mask);

//          switch(mask) {
//              case GPGGA:
//                  break;
//              case GPGLL:
//                  break;
//              case GPRMC:
//                  break;
//              case GPVTG:
//                  break;
//              case GPGSA:
//                  break;
//              case GPGSV:
//                  break;
//              case GPZDA:
//                  break;
//              case GPGBS:
//                  break;
//              case UNKNOWN:
//                  printf("Unknown detected \n");
//                  break;
//              default:
//                  break;
//          }
            return; // Note: Keep return to ensure the next line is read
        }
    }
}