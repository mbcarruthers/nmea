#ifndef __NMEA_PARSE__
#define __NMEA_PARSE__

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "sentences.h"

#ifndef EOF
#define EOF '\0'
#endif
#define TOKEN_SIZE 256



char nmea[] = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";

void parse_sentence(char * restrict str) {
  char token[TOKEN_SIZE] = {0}, *strp = str;
  struct NMEA_sentenceType * sentence;
  while(*strp) {
      if(*strp == '$') {
          strp++;
          int i = 0;
          while(*strp != ',' && *strp != '\0') {
              token[i++] = *strp++; // copy char move pointer. beautiful
          }
          token[i] = '\0';
          NMEA_Sentence mask = nmea_to_mask(token);
          printf("Token %s \n Mask %x \n",
          token, mask);
          return; // Note: Keep return to ensure the next line is read
      }
  }
}
// __NMEA_PARSE__
#endif