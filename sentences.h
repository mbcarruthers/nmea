#ifndef NMEA_SENTENCES_H__
#define NMEA_SENTENCES_H__

#include <stdint.h>

// Note: if no comment on field type it should be self explanatory
// Note cont: or previously written

// Todo: Should these just be defines?
typedef enum {
    GPGGA = (1 << 0) , // GPS 0b00000001
    GPGLL = (1 << 1),  // Latitude && longitude  0b00000010
    GPRMC = (1 << 2), // Nav data 0b00000100
    GPVTG = (1 << 3), // track made good and ground speed 0b00001000
    GPGSA = (1 << 4), // GPS DOP and active satellites 0b00010000
    GPGSV = (1 << 5), // satellites within view 0b00100000
    GPZDA = (1 << 6), // Time and Date 0b01000000
    GPGBS = (1 << 7), // GNSS Satellite Fault Detection 0b10000000
    UNKNOWN = 0x00
} NMEA_Mask;

// GPS Fix data
struct GPGGA_Sentence {
    uint32_t utc_time; // HHMMSS(UTC Time) Todo: Find RFC for this format
    float latitude; // latitude in decimal degrees
    char lat_dir; // 'N' or 'S'
    float longitude; // longitude in decimal degrees
    char lon_dir; // 'E' or 'W'
    uint8_t fix_quality; // Note: quality : 0 = invalid, 1 = GPS fix, 2 = DGPS Fix
    uint8_t num_satellites; // number of satellites available?
    float hdop; // 'Horizontal Dilution of Precision'
    float altitude; // Altitude in meters
    float geoidal_seperation; // geoidal seperation in meters
};

// Latitude && Longitude
struct GPGLL_Sentence {
    float latitude;
    char lat_dir;
    float longitude;
    char lon_dir;
    uint32_t utc_time; // HHMMSS
    char status; // A = valid, V = invalid
};

// minimum  nav data
struct GPRMC_Sentence {
    uint32_t utc_time; // HHMMSS
    char status; // A  = valid , v = invalid
    float latitude;
    char lat_dir;
    float longitude;
    char lon_dir;
    float speed_over_ground;
    float course_over_ground;
    uint32_t date; // DDMMYY Todo: Find RFC for this format
    float magnetic_variation; // Magnetic variation in degrees
    char var_dir; // variation direction, east(E) or west(W)
};

// track made good && ground speed
struct GPVTG_Sentence {
    float true_track; // true track made good (degrees)
    char true_indicator; // T
    float magnetic_track; // Magnetic track made good (degrees)
    char magnetic_indicator;  // M
    float speed_knots; // Speed over ground (knots)
    char knots_indicator; // 'N'
    float speed_kmph; // speed over ground km/h
    char kmph_indicator; // 'K'
};

// GPS DOP and active satellites
struct GPGSA_Sentence {
    char mode; // Manual or Automatic
    uint8_t fix_type; // 1 = none, 2 = 2D fix, 3 = 3D fix
    uint8_t satellites[12]; // PRN's of satellites
    float pdop; // position dilution of precision
    float hdop; // horizontal dilution od precision
    float vdop; // vertical dilution of prescision
};

// Satellites in view
struct GPGSV_Sentence {
    uint8_t total_messages; // self explanatory
    uint8_t message_number;
    uint8_t satellites_in_view;
    struct {
        uint32_t ptn; // Satellite PRN number
        uint32_t elevation; // elevation in degrees
        uint32_t azimuth; // azimuth in degrees
        uint8_t snr; // signal-to-noise ratio (0-99dB)
    } satellite_info[4];
};

// Time and date
struct GPZDA_Sentence {
    uint32_t utc_time; //HHMMSS
    uint8_t day;
    uint8_t month;
    uint16_t year;
    uint8_t local_hour_offset;
    uint8_t local_minute_offset;
};

// GNSS Satellite Fault Detection
struct GPGBS_Sentence {
    uint32_t utc_time; //HHMMSS
    float horizontal_error; // horizontal accuracy in meters
    float vertical_error; // vertical accuracy in meters
    float position_error; // overall position accuracy in meters
};


typedef struct {
    NMEA_Mask nmea;
    union {
        struct GPGGA_Sentence gpgga;
        struct GPGLL_Sentence gpgll;
        struct GPRMC_Sentence gprmc;
        struct GPVTG_Sentence gpvtg;
        struct GPGSA_Sentence gpgsa;
        struct GPGSV_Sentence gpgsv;
        struct GPZDA_Sentence gpzda;
        struct GPGBS_Sentence gpgbs;
    } value;
} NMEA_SentenceType;

// determines what type of NMEA sentence it is based on the 3rd and fourth letters
// with a little bit of bitwise hackery
NMEA_Mask nmea_to_mask(const char * restrict token);

// NMEA_SENTENCES_H__
#endif