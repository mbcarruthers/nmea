#include "nmea.h"


size_t parse_csv_line(const char * line, char tokens[MAX_TOKENS][TOKEN_LENGTH]) {
    size_t token_count = 0;
    const char *start = line;
    const char *end;

    while ((end = strchr(start, ',')) != NULL) {
        size_t length = end - start; // Length of the current token


        if (length >= TOKEN_LENGTH) {
            fprintf(stderr, "Token exceeds maximum size.\n");
            return 0; // Error
        }


        strncpy(tokens[token_count], start, length);
        tokens[token_count][length] = '\0'; // Null-terminate
        token_count++;

        // move to the next token
        start = end + 1;


        if (token_count >= MAX_TOKENS) {
            fprintf(stderr, "Too many tokens in the line.\n");
            return token_count; // Return the count so far
        }
    }

    // handle the last token (after the final comma or if no commas)
    if (*start != '\0') {
        strncpy(tokens[token_count], start, TOKEN_SIZE - 1);
        tokens[token_count][TOKEN_LENGTH - 1] = '\0'; // Null-terminate
        token_count++;
    }

    return token_count;
}

static inline float parse_float(const char * token) {
    return token ? strtof(token, NULL) : 0.0f;
}

static inline uint32_t parse_uint32(const char * token) {
    return token ? (uint32_t) strtoul(token, NULL, 10) : 0;
}

NMEA_SentenceType parser(char * restrict str) {
    char tokens[MAX_TOKENS][TOKEN_LENGTH];
    // Note: unused and known
    const size_t count = parse_csv_line(str, tokens);

    // now all values are comma seperated
    NMEA_SentenceType sentence = {0};
    char * sentence_type = tokens[0] + sizeof(char); // string variable for NMEA sentence type - sizeof(char) for explicitness

    sentence.nmea = nmea_to_mask(sentence_type);
    // Todo: declare iterator(not i) = 0 in switch,i++ indexs.

#ifdef DEBUG
    printf("[DEBUG] File: %s, Line: %d\n"
       "        Token: %s\n"
       "        Mask:  0x%02X\n",
       __FILE__, __LINE__,
       sentence_type, sentence.nmea);
#endif

    switch(sentence.nmea) {
        case GPGGA:
            sentence.value.gpgga.utc_time = parse_uint32(tokens[1]);
            sentence.value.gpgga.latitude = parse_float(tokens[2]);
            sentence.value.gpgga.lat_dir = (char)(*tokens[3]);
            sentence.value.gpgga.longitude = parse_float(tokens[4]);
            sentence.value.gpgga.lon_dir = (char)(*tokens[5]);
            sentence.value.gpgga.fix_quality = (uint8_t)(*tokens[6]);
            sentence.value.gpgga.num_satellites = (uint8_t)(*tokens[7]);
            sentence.value.gpgga.hdop = parse_float(tokens[8]);
            sentence.value.gpgga.altitude = parse_float(tokens[9]);
            sentence.value.gpgga.geoidal_seperation = parse_float(tokens[10]);
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
            sentence.value.gprmc.latitude = parse_float(tokens[2]);
            sentence.value.gprmc.lat_dir = (char)(*tokens[3]);
            sentence.value.gprmc.speed_over_ground = parse_float(tokens[4]);
            sentence.value.gprmc.course_over_ground = parse_float(tokens[5]);
            sentence.value.gprmc.date = parse_uint32(tokens[6]);
            sentence.value.gprmc.magnetic_variation = parse_float(tokens[7]);
            sentence.value.gprmc.var_dir = (char)(*tokens[8]);
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
            sentence.value.gpgsa.mode = (char)(*tokens[1]);
            sentence.value.gpgsa.fix_type = (uint8_t)(*tokens[2]);
            for (int i = 0; i < 12; i++) {
                if (tokens[3 + i][0] == '\0') {
                    // Note: Empty tokens given 0 value in gpgsa.satellites
                    sentence.value.gpgsa.satellites[i] = 0;
                } else {
                    sentence.value.gpgsa.satellites[i] = (uint8_t)atoi(tokens[3 + i]);
                }
            }
            sentence.value.gpgsa.pdop = parse_float(tokens[15]);
            sentence.value.gpgsa.hdop = parse_float(tokens[16]);
            sentence.value.gpgsa.vdop = parse_float(tokens[17]);
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
            return sentence;  // todo better solution
            break;
        default:
            printf("------------------------Default ------------------\n");
            return sentence; // todo - better solution
            break;
    }

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