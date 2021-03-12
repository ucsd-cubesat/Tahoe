#ifndef NMEA_H
#define NMEA_H

#include <stdint.h>

/**
 * @brief Calculates the checksum of an NMEA sentence
 * @param nmea An NMEA sentence
 * @return The checksum; 0 if the sentence is invalid
 */
uint8_t nmea_calculate_checksum(const char *nmea);

/**
 * @brief Parses the checksum out of an NMEA sentence
 * @param nmea An NMEA sentence
 * @return The checksum at the end of the sentence; undefined if the sentence is
 *         invalid
 */
uint8_t nmea_parse_checksum(const char *nmea);

/**
 * @brief Verifies an NMEA sentence using its checksum
 * @param nmea An NMEA sentence
 * @return true if the NMEA sentence is valid, false otherwise
 */
bool nmea_verify_checksum(const char *nmea);

#endif /* NMEA_H */