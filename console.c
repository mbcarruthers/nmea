#include <stdio.h>
#include "console.h"
#include "cbuf.h"

// =======================================================================================
//                                UTC-TIME DISPLAY HACKERY
//                                    | LEAVE ALONE |

// // breaks uint32_t into 3 seperate groups of 2 digits in respective order
#define UNPACK_UINT32_TO_VALUES(n) (n) / 10000, ((n) / 100) % 100, (n) % 100

// turns the above 3 digits into a uint8_t array
#define WRAP_AS_ARGS(n) (uint8_t[]){UNPACK_UINT32_TO_VALUES(n)}

// generates a null-terminated character array seperated by :'s
// - turns index to ascii  , adds comma and null terminating \0
#define FORMAT_TIME(args) (char[]){ \
    ((args)[0] / 10 + '0'), ((args)[0] % 10 + '0'), ':', \
    ((args)[1] / 10 + '0'), ((args)[1] % 10 + '0'), ':', \
    ((args)[2] / 10 + '0'), ((args)[2] % 10 + '0'), '\0' }


// PARSE_UTC - parses a UTC value
// uses all of the above
// input must be 6 digit uint32_t
#define PARSE_UTC(c) FORMAT_TIME(WRAP_AS_ARGS(c))

// =======================================================================================

void print_GPGGA_Sentence(const struct GPGGA_Sentence * gpgga) {
    printf("GPGGA Sentence:\n");
    printf("  UTC Time: %s \n", PARSE_UTC(gpgga->utc_time));
    printf("  Latitude: %.6f %c\n", gpgga->latitude, gpgga->lat_dir);
    printf("  Longitude: %.6f %c\n", gpgga->longitude, gpgga->lon_dir);
    printf("  Fix Quality: %u\n", gpgga->fix_quality);
    printf("  Number of Satellites: %u\n", gpgga->num_satellites);
    printf("  HDOP: %.2f\n", gpgga->hdop);
    printf("  Altitude: %.2f meters\n", gpgga->altitude);
    printf("  Geoidal Separation: %.2f meters\n", gpgga->geoidal_seperation);
}

void print_GPGLL_Sentence(const struct GPGLL_Sentence * gpgll) {
    printf("GPGLL Sentence:\n");
    printf("  Latitude: %.6f %c\n", gpgll->latitude, gpgll->lat_dir);
    printf("  Longitude: %.6f %c\n", gpgll->longitude, gpgll->lon_dir);
    printf("  UTC Time: %s \n", PARSE_UTC(gpgll->utc_time));
    printf("  Status: %c\n", gpgll->status);
}

void print_GPRMC_Sentence(const struct GPRMC_Sentence * gprmc) {
    printf("GPRMC Sentence:\n");
    printf("  UTC Time: %s\n", PARSE_UTC(gprmc->utc_time));
    printf("  Status: %c\n", gprmc->status);
    printf("  Latitude: %.6f %c\n", gprmc->latitude, gprmc->lat_dir);
    printf("  Longitude: %.6f %c\n", gprmc->longitude, gprmc->lon_dir);
    printf("  Speed over Ground: %.2f knots\n", gprmc->speed_over_ground);
    printf("  Course over Ground: %.2f degrees\n", gprmc->course_over_ground);
    printf("  Date: %06u\n", gprmc->date);
    printf("  Magnetic Variation: %.2f %c\n", gprmc->magnetic_variation, gprmc->var_dir);
}

void print_GPVTG_Sentence(const struct GPVTG_Sentence * gpvtg) {
    printf("GPVTG Sentence:\n");
    printf("  True Track: %.2f %c\n", gpvtg->true_track, gpvtg->true_indicator);
    printf("  Magnetic Track: %.2f %c\n", gpvtg->magnetic_track, gpvtg->magnetic_indicator);
    printf("  Speed (knots): %.2f %c\n", gpvtg->speed_knots, gpvtg->knots_indicator);
    printf("  Speed (km/h): %.2f %c\n", gpvtg->speed_kmph, gpvtg->kmph_indicator);
}

void print_GPGSA_Sentence(const struct GPGSA_Sentence * gpgsa) {
    printf("GPGSA Sentence:\n");
    printf("  Mode: %c\n", gpgsa->mode);
    printf("  Fix Type: %u\n", gpgsa->fix_type);
    printf("  Satellites: ");
    for (int i = 0; i < 12; i++) {
        printf("%u ", gpgsa->satellites[i]);
    }
    printf("\n  PDOP: %.2f\n", gpgsa->pdop);
    printf("  HDOP: %.2f\n", gpgsa->hdop);
    printf("  VDOP: %.2f\n", gpgsa->vdop);
}

void print_GPGSV_Sentence(const struct GPGSV_Sentence * gpgsv) {
    printf("GPGSV Sentence:\n");
    printf("  Total Messages: %u\n", gpgsv->total_messages);
    printf("  Message Number: %u\n", gpgsv->message_number);
    printf("  Satellites in View: %u\n", gpgsv->satellites_in_view);
    for (int i = 0; i < 4; i++) {
        printf("    Satellite %d:\n", i + 1);
        printf("      PRN: %d\n", gpgsv->satellite_info[i].ptn);
        printf("      Elevation: %d degrees\n", gpgsv->satellite_info[i].elevation);
        printf("      Azimuth: %d degrees\n", gpgsv->satellite_info[i].azimuth);
        printf("      SNR: %u dB\n", gpgsv->satellite_info[i].snr);
    }
}

void print_GPZDA_Sentence(const struct GPZDA_Sentence * gpzda) {
    printf("GPZDA Sentence:\n");
    printf("  UTC Time: %s\n", PARSE_UTC(gpzda->utc_time));
    printf("  Date: %02u/%02u/%04u\n", gpzda->day, gpzda->month, gpzda->year);
    printf("  Local Hour Offset: %d\n", gpzda->local_hour_offset);
    printf("  Local Minute Offset: %d\n", gpzda->local_minute_offset);
}

void print_GPGBS_Sentence(const struct GPGBS_Sentence * gpgbs) {
    printf("GPGBS Sentence:\n");
    printf("  UTC Time: %s\n", PARSE_UTC(gpgbs->utc_time));
    printf("  Horizontal Error: %.2f meters\n", gpgbs->horizontal_error);
    printf("  Vertical Error: %.2f meters\n", gpgbs->vertical_error);
    printf("  Position Error: %.2f meters\n", gpgbs->position_error);
}

void printall(void) {
    const NMEA_SentenceType * curr = Front();
    const NMEA_SentenceType * end = Back();
    while(curr != end) {
        switch(curr->nmea) {
            case GPGGA:
                print_GPGGA_Sentence(&curr->value.gpgga);
                break;
            case GPGLL:
                print_GPGLL_Sentence(&curr->value.gpgll);
                break;
            case GPRMC:
                print_GPRMC_Sentence(&curr->value.gprmc);
                break;
            case GPVTG:
                print_GPVTG_Sentence(&curr->value.gpvtg);
                break;
            case GPGSA:
                print_GPGSA_Sentence(&curr->value.gpgsa);
                break;
            case GPGSV:
                print_GPGSV_Sentence(&curr->value.gpgsv);
                break;
            case GPZDA:
                print_GPZDA_Sentence(&curr->value.gpzda);
                break;
            case GPGBS:
                print_GPGBS_Sentence(&curr->value.gpgbs);
                break;
            case UNKNOWN:
                printf("Unknown printall called\n");
                break;
            default:
                printf("default printall called \n");
                break;
        }
        curr++;
    }
}