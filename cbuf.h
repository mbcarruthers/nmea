#ifndef CBUF_H__
#define CBUF_H__

#include <stddef.h>
#include "sentences.h"

size_t capacity(void);

void push(NMEA_SentenceType sentence);

void print_GPGGA_Sentence(const struct GPGGA_Sentence * gpgga);

void print_GPGLL_Sentence(const struct GPGLL_Sentence * gpgll);

void print_GPRMC_Sentence(const struct GPRMC_Sentence * gprmc);

void print_GPVTG_Sentence(const struct GPVTG_Sentence * gpvtg);

void print_GPGSA_Sentence(const struct GPGSA_Sentence * gpgsa);

void print_GPGSV_Sentence(const struct GPGSV_Sentence * gpgsv);

void print_GPZDA_Sentence(const struct GPZDA_Sentence * gpzda);

void print_GPGBS_Sentence(const struct GPGBS_Sentence * gpgbs);

void printall(void);

// CBUF_H__
#endif