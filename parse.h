//
// Created by mbc on 12/17/24.
//

/*
 * @ parse.h - Page 156 from CPL, essentially. Some minor nuances changed
 */
#ifndef FN_GRAVEYARD__
#define FN_GRAVEYARD__
#include <stdio.h>
#include <stdarg.h>


// Note: Good example of parsing pattern
// Note: And use variadic argument functionality
// page 156 CPL
void parse(char * fmt, ...) {
    va_list ap; // 1. Create list variable
    char *p, *sval;
    int ival;
    double dval;

    // starts the variable argument list moving
    // must be called before va_list(ap) is used
    va_start(ap, fmt); // 2. add arguements to variable
    for(p = fmt; *p;p++) {
        if(*p != '%') {
            putchar(*p);
            continue;
        }
        // 3. Test types on argument
        switch(*++p) {
            case 'd':
                ival = va_arg(ap, int); // 3
                printf("%d", ival);
                break;
            case 'f':
                dval = va_arg(ap, double); // 3
                printf("%f", dval);
                break;
            case 's':
                // 3
                for(sval = va_arg(ap, char*); *sval;sval++) {
            putchar(*sval);
        }
                break;
            default:
                putchar(*p);
                break;
        }
    }
    printf("\n");
    va_end(ap); // 4 clearing arguments
}


// old_parse_csv - from nmea.c
//size_t parse_csv_line(const char * line, char tokens[MAX_TOKENS][TOKEN_LENGTH]) {
//    size_t token_count = 0;
//    const char *start = line;
//    const char *end;
//
//    while((end = strchr(start, ',')) != NULL) {
//        size_t length = end - start; // Length of the current token
//
//
//        if(length >= TOKEN_LENGTH) {
//            fprintf(stderr, "Token exceeds maximum size.\n");
//            return 0; // Error
//        }
//
//
//        strncpy(tokens[token_count], start, length);
//        tokens[token_count][length] = '\0'; // Null-terminate
//        token_count++;
//
//        // move to the next token
//        start = end + 1;
//
//
//        if(token_count >= MAX_TOKENS) {
//            fprintf(stderr, "Too many tokens in the line.\n");
//            return token_count; // Return the count so far
//        }
//    }
//
//    // handle the last token (after the final comma or if no commas)
//    if (*start != '\0') {
//        strncpy(tokens[token_count], start, TOKEN_SIZE - 1);
//        tokens[token_count][TOKEN_LENGTH - 1] = '\0'; // Null-terminate
//        token_count++;
//    }
//
//    return token_count;
//}

// initial throwaway buffer

//#include <stdio.h>
//#include <stdlib.h>
//#include "cbuf.h"
//
//#define BUFFER_LENGTH 24
//
//
//static NMEA_SentenceType recvd[BUFFER_LENGTH]; // Ring buffer -> log
//static NMEA_SentenceType * front = recvd;
//static NMEA_SentenceType * back = &recvd[BUFFER_LENGTH];
//
//size_t capacity(void) {
//    return (size_t)(front >= back) ? (front - back) : ((sizeof(recvd) / sizeof(recvd[0])) - (back- front));
//}
//
//void push(NMEA_SentenceType sentence) {
//    *front = sentence;
//
//
//    if (++front == back) {
//        front = recvd; // cheap circular buffer
//    }
//    printf("Capacity %zu \n",
//           capacity());
//}


// parse_sentence - unused currently. Keep for now. 12.18.24
//void parse_sentence(char * str) {
//    char token[TOKEN_SIZE] = {0}, *strp = str;
//    char tokens[MAX_TOKENS][TOKEN_LENGTH];
////    struct NMEA_SentenceType *sentence;
//    size_t count = parse_csv_line(str, tokens);
//    printf("Count of csv file %zu \n",
//           count); // to get compiler off my back abt an unused fn
//
//
//    while (*strp) {
//        if (*strp == '$') {
//            strp++;
//            int i = 0;
//            while (*strp != ',' && *strp != '\0') {
//                token[i++] = *strp++; // copy char move pointer. beautiful
//            }
//            token[i] = '\0';
//            NMEA_Mask mask = nmea_to_mask(token);
//            printf("Token %s \n Mask %x \n",
//                   token, mask);
//
////          switch(mask) {
////              case GPGGA:
////                  break;
////              case GPGLL:
////                  break;
////              case GPRMC:
////                  break;
////              case GPVTG:
////                  break;
////              case GPGSA:
////                  break;
////              case GPGSV:
////                  break;
////              case GPZDA:
////                  break;
////              case GPGBS:
////                  break;
////              case UNKNOWN:
////                  printf("Unknown detected \n");
////                  break;
////              default:
////                  break;
////          }
//            return; // Note: Keep return to ensure the next line is read
//        }
//    }
//}
#endif // FN_GRAVEYARD __
