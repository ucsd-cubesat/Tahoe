#ifndef NMEA_H
#define NMEA_H

#include <stdint.h>

/* This library supports NMEA 0183 (version 4.10) */

/**
 * @brief Calculates the checksum of an NMEA sentence
 * @param nmea An NMEA sentence, null-terminated, may omit the starting '$',
 *             the '*HH' checksum, and the <CRLF> ending
 * @return The checksum numeric value
 */
uint8_t nmea_calculate_checksum(const char *nmea);

/**
 * @brief Parses the checksum out of an NMEA sentence
 * @param nmea An NMEA sentence, null-terminated, needing only contain the '*HH'
 *             checksum
 * @return The checksum at the end of the sentence; 0 if no checksum is found or
 *         is incorrectly formatted
 */
uint8_t nmea_parse_checksum(const char *nmea);

/**
 * @brief Verifies an NMEA sentence using its checksum
 * @param nmea An NMEA sentence, null-terminated, may omit the starting '$', and
 *             the <CRLF> ending.
 * @return true if the NMEA sentence is valid, meaning its calculated checksum
 *         matches the reported checksum; false otherwise
 */
bool nmea_verify_checksum(const char *nmea);

/**
 * Enumerates the type of an NMEA sentence 
 */
enum NMEA {
    NMEA_RMC,
    NMEA_VTG,
    NMEA_GGA,
    NMEA_GSA,
    NMEA_GLL,
    NMEA_UNKNOWN
};

/**
 * Finds the type of an NMEA sentence
 */
NMEA nmea_get_type(const char *nmea);

/**
 * @brief Parses an RMC-type sentence, storing pointers to the various strings
 *        of data to the arguments. Empty srings mean no data, and any argument
 *        may be null
 * @param rmc The RMC-type sentence, which will be modified
 * @param utc    UTC time of position, as hhmmss.ss
 * @param status 'A' for valid, 'V' for warning
 * @param lat    Latitude, as llll.ll
 * @param lat_ns Latitude direction, 'N' for north or 'S' for south
 * @param lon    Longitude, as yyyyy.yy
 * @param lon_ew Longitude direction, 'E' for east or 'W' for west
 * @param speed  Speed over ground, in knots
 * @param track  Track made good, in degrees true
 * @param date   Date, as ddmmyy
 * @param mag    Magnetic variation, in degrees
 * @param mag_ew Magnetic variation direction, 'E' for east or 'W' for west
 */
void nmea_parse_rmc(char *rmc, const char **utc, const char **status, 
                    const char **lat, const char **lat_ns, const char **lon,
                    const char **lon_ew, const char **vel, const char **track,
                    const char **date, const char **mag, const char **mag_ew);

/**
 * @brief Parses an VTG-type sentence, storing pointers to the various strings
 *        of data to the arguments. Empty srings mean no data, and any argument
 *        may be null
 * @param vtg The VTG-type sentence, which will be modified
 * @param deg  Course over ground, in degrees true
 * @param mag  Course over ground, in degrees magnetic
 * @param knot Speed over ground, in knots
 * @param kmhr Speed over ground, in km/hr
 */
void nmea_parse_vtg(char *vtg, const char **deg, const char **mag,
                    const char **knot, const char kmhr);

/**
 * Describes the quality of a GPS fix
 */
enum GPS_QUALITY: char {
    // No fix available
    GPS_QUALITY_NA = '0',

    // Fix is available
    GPS_QUALITY_FIX = '1',

    // Fix is differential
    GPS_QUALITY_DIFF_FIX = '2',

    // Fix is PPS
    GPS_QUALITY_PPS_FIX = '3',

    // Fix is real-time kinetic
    GPS_QUALITY_RTK_FIX = '4',

    // Fix is float real-time kinetic
    GPS_QUALITY_FLOAT_RTK = '5',

    // Fix is estimated (dead-reckoning)
    GPS_QUALITY_ESTIMATED = '6',

    // Fix is manually inputed
    GPS_QUALITY_MANUAL = '7',

    // Fix is simulated
    GPS_QUALITY_SIMUL = '8'
};

/**
 * @brief Parses an GGA-type sentence, storing pointers to the various strings
 *        of data to the arguments. Empty srings mean no data, and any argument
 *        may be null
 * @param gga The GGA-type sentence, which will be modified
 * @param utc     UTC time of position, as hhmmss.ss
 * @param lat     Latitude, as llll.ll
 * @param lat_ns  Latitude direction, 'N' for north or 'S' for south
 * @param lon     Longitude, as yyyyy.yy
 * @param lon_ew  Longitude direction, 'E' for east or 'W' for west
 * @param quality Quality indicator, see GPS_QUALITY
 * @param num     Number of sattelites in use, ranging [0, 12]
 * @param hdop    Horizontal dilution of precision, in meters
 * @param alt     Antenna altitude from mean-sea-level, in meters
 * @param geoid   Geoidal seperation (between WGS-84 earth ellipsoid and
 *                mean-sea-level), in meters
 * @param dage    Age of differential GPS data, in seconds since last SC104
 *                update
 * @param ref     Differential reference station ID, ranging [0, 1023]
 */
void nmea_parse_gga(char *gga, const char **utc, const char **lat,
                    const char **lat_ns, const char **lon, const char **lon_ew,
                    const char **quality, const char **num,
                    const char **hdop, const char **alt,
                    const char **geoid, const char **dage, const char** ref);

/**
 * @brief Parses an GSA-type sentence, storing pointers to the various strings
 *        of data to the arguments. Empty srings mean no data, and any argument
 *        may be null
 * @param gsa The GSA-type sentence, which will be modified
 * @param select Selection mode, 'M' for manual or 'A' for automatic
 * @param mode   Fix mode, '1' for no fix or '2' for 2D fix or '3' for 3D fix
 * @param idN    ID of the Nth satellite used for fix
 * @param pdop   Position of dilution of precision
 * @param hdop   Horizontal of dilution of precision
 * @param vdop   Vertical of dilution of precision
 */
void nmea_parse_gsa(char *gsa, const char **select, const char **mode,
                    const char **id1, const char **id2, const char **id3,
                    const char **id4, const char **id5, const char **id6,
                    const char **id7, const char **id8, const char **id9,
                    const char **id10, const char **id11, const char **id12,
                    const char **pdop, const char **hdop, const char **vdop);

/**
 * @brief Parses an GLL-type sentence, storing pointers to the various strings
 *        of data to the arguments. Empty srings mean no data, and any argument
 *        may be null
 * @param gll The GLL-type sentence, which will be modified
 * @param lat    Latitude, as llll.ll
 * @param lat_ns Latitude direction, 'N' for north or 'S' for south
 * @param lon    Longitude, as yyyyy.yy
 * @param lon_ew Longitude direction, 'E' for east or 'W' for west
 * @param utc    UTC time of position, as hhmmss.ss
 * @param status 'A' for valid, 'V' for invalid
 */
void nmea_parse_gll(char *gll, const char **lat, const char **lat_ns,
                    const char **lon, const char **lon_ew, const char **utc,
                    const char **status);

#endif /* NMEA_H */