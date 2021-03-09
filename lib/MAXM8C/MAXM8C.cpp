#include "MAXM8C.h"

#define ASCII_A 65
#define ASCII_0 48

/**
 * Parses any upercase hex digits
 */
uint8_t parseHexDigit(const char digit) {
    if (digit < ASCII_A) {
        return digit - ASCII_0;
    }
    else {
        return 0x0A + digit - ASCII_A;
    }
}

/**
 * Finds and parses the checksum at the end of an nmea sentence. MUST BE NULL
 * TERMINATED
 */
uint8_t parseChecksum(const char *checksum) {
    const char highDigit = *checksum;
    const char lowDigit = *(checksum + 1);
    return (parseHexDigit(highDigit) << 4) | parseHexDigit(lowDigit);
}

/**
 * Calculates the checksum of an NMEA sentence. MUST BE NULL TERMINATED
 */
uint8_t calculateChecksum(const char *nmea) {
    const char *p = nmea;
    uint8_t checksum = 0;
    while (*p != '*' && *p != '\0') {
        checksum ^= *p;
    }
    return checksum;
}

uint8_t MAXM8C::readSentence(char *nmea, size_t len) {
    while (Serial1.read() != '$');

    size_t numRead = Serial1.readBytesUntil('*', nmea, len);
    nmea[numRead] = '\0';

    char checksum[3];
    Serial1.readBytesUntil('\r', checksum, 3);
    checksum[2] = 0;

    uint8_t parsedChecksum = parseChecksum(checksum);
    uint8_t calculatedChecksum = calculateChecksum(nmea);

    return parsedChecksum == calculatedChecksum ? 0 : parsedChecksum;
}