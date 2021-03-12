#include "NMEA.h"

#define ASCII_A 65
#define ASCII_0 48

/**
 * Parses any upercase hex digits
 */
uint8_t parse_hex_digit(const char digit) {
    if (digit < ASCII_A) {
        return digit - ASCII_0;
    }
    else {
        return 0x0A + digit - ASCII_A;
    }
}

uint8_t nmea_calculate_checksum(const char *nmea) {
    const char *p = nmea;
    uint8_t checksum = 0;

    if (*p == '$') {
        p++; // skips '$'
    }

    while (*p != '*' && *p != 0) {
        checksum ^= *p;
        p++;
    }

    return checksum;
}

uint8_t nmea_parse_checksum(const char *nmea) {
    const char *p = nmea;
    uint8_t checksum = 0;

    // skip to checksum
    while (*p != '*' && *p != 0) {
        p++;
    }
    if (*p == 0) {
        return 0;
    }
    p++; // skip '*'

    // parse checksum
    const char highDigit = p[0];
    const char lowDigit = p[1];
    checksum = (parse_hex_digit(highDigit) << 4) | parse_hex_digit(lowDigit);

    return checksum;
}

bool nmea_verify_checksum(const char *nmea) {
    return nmea_calculate_checksum(nmea) == nmea_parse_checksum(nmea);
}