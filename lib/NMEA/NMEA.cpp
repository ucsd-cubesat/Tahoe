#include "NMEA.h"

#define ASCII_A 65
#define ASCII_0 48

/** UNVETTED
 * Parses any upercase hex digits into their numeric values
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

    // parse checksum
    p++; // skip '*'
    const char highDigit = p[0];
    const char lowDigit = p[1];
    checksum = (parse_hex_digit(highDigit) << 4) | parse_hex_digit(lowDigit);

    return checksum;
}

bool nmea_verify_checksum(const char *nmea) {
    return nmea_calculate_checksum(nmea) == nmea_parse_checksum(nmea);
}

/** UNVETTED
 * Fast string-prefix matcher
 */
bool starts_with(const char *s, const char* prefix) {
    if (s == prefix) return true;
    while (*s && *prefix && // no nulls encountered
           *s == *prefix) { // corresponding characters still match
        s++; prefix++;
    }
    // either we hit a null, or two corresponding characters did not match
    return *prefix == 0 || *s == *prefix;
}

NMEA nmea_get_type(const char *nmea) {
    if (*nmea == '$') nmea++; // skip '$'
    nmea += 2; // skip ID
    if (starts_with(nmea, "RMC")) return NMEA_RMC;
    if (starts_with(nmea, "VTG")) return NMEA_VTG;
    if (starts_with(nmea, "GGA")) return NMEA_GGA;
    if (starts_with(nmea, "GSA")) return NMEA_GSA;
    if (starts_with(nmea, "GLL")) return NMEA_GLL;
    return NMEA_UNKNOWN;
}

/** UNVETTED
 * @brief Captures the position of the next field in an NMEA sentence, replacing
 *        the following seperator ',' with a null terminator
 * @param buff A reference into the buffer, pointing at the current field. This
 *             is modified during processing, but should NOT be modified by the
 *             user until it is no longer needed
 * @param loc  A pointer to store the location of the current field. This will
 *             point INTO buff, so buff should NOT be modified by the user
 */
void nmea_get_next_field(char* &buff, const char* *loc) {
    if (loc != nullptr) *loc = buff; // store the current field location
    while (*buff != ',' && *buff != 0) buff++; // find the next field
    *buff = 0; // replace the ',' with null terminator
    buff++; // skip ahead 1 to the beginning of the next field
}

void nmea_parse_rmc(char *rmc, const char **utc, const char **status, 
                    const char **lat, const char **lat_ns, const char **lon,
                    const char **lon_ew, const char **speed, const char **track,
                    const char **date, const char **mag, const char **mag_ew) {
    // skips "[$]...," as a fun side-effect rmc will appear as a string
    // containing the prefix of the string
    nmea_get_next_field(rmc, nullptr);
    // parse fields in order
    nmea_get_next_field(rmc, utc);
    nmea_get_next_field(rmc, status);
    nmea_get_next_field(rmc, lat);
    nmea_get_next_field(rmc, lat_ns);
    nmea_get_next_field(rmc, lon);
    nmea_get_next_field(rmc, lon_ew);
    nmea_get_next_field(rmc, speed);
    nmea_get_next_field(rmc, track);
    nmea_get_next_field(rmc, date);
    nmea_get_next_field(rmc, mag);
    nmea_get_next_field(rmc, mag_ew);
}

void nmea_parse_vtg(char *vtg, const char **deg, const char **mag,
                    const char **knot, const char **kmhr) {
    nmea_get_next_field(vtg, nullptr);
    nmea_get_next_field(vtg, deg);
    nmea_get_next_field(vtg, nullptr);
    nmea_get_next_field(vtg, mag);
    nmea_get_next_field(vtg, nullptr);
    nmea_get_next_field(vtg, knot);
    nmea_get_next_field(vtg, nullptr);
    nmea_get_next_field(vtg, kmhr);
}

void nmea_parse_gga(char *gga, const char **utc, const char **lat,
                    const char **lat_ns, const char **lon, const char **lon_ew,
                    const char **quality, const char **num,
                    const char **hdop, const char **alt,
                    const char **geoid, const char **dage, const char** ref) {
    nmea_get_next_field(gga, nullptr);
    nmea_get_next_field(gga, utc);
    nmea_get_next_field(gga, lat);
    nmea_get_next_field(gga, lat_ns);
    nmea_get_next_field(gga, lon);
    nmea_get_next_field(gga, lon_ew);
    nmea_get_next_field(gga, quality);
    nmea_get_next_field(gga, num);
    nmea_get_next_field(gga, hdop);
    nmea_get_next_field(gga, alt);
    nmea_get_next_field(gga, nullptr);
    nmea_get_next_field(gga, geoid);
    nmea_get_next_field(gga, nullptr);
    nmea_get_next_field(gga, dage);
    nmea_get_next_field(gga, ref);
}

void nmea_parse_gsa(char *gsa, const char **select, const char **mode,
                    const char **id1, const char **id2, const char **id3,
                    const char **id4, const char **id5, const char **id6,
                    const char **id7, const char **id8, const char **id9,
                    const char **id10, const char **id11, const char **id12,
                    const char **pdop, const char **hdop, const char **vdop) {
    nmea_get_next_field(gsa, nullptr);
    nmea_get_next_field(gsa, select);
    nmea_get_next_field(gsa, mode);
    nmea_get_next_field(gsa, id1);
    nmea_get_next_field(gsa, id2);
    nmea_get_next_field(gsa, id3);
    nmea_get_next_field(gsa, id4);
    nmea_get_next_field(gsa, id5);
    nmea_get_next_field(gsa, id6);
    nmea_get_next_field(gsa, id7);
    nmea_get_next_field(gsa, id8);
    nmea_get_next_field(gsa, id9);
    nmea_get_next_field(gsa, id10);
    nmea_get_next_field(gsa, id11);
    nmea_get_next_field(gsa, id12);
    nmea_get_next_field(gsa, pdop);
    nmea_get_next_field(gsa, hdop);
    nmea_get_next_field(gsa, vdop);
}

void nmea_parse_gll(char *gll, const char **lat, const char **lat_ns,
                    const char **lon, const char **lon_ew, const char **utc,
                    const char **status) {
    nmea_get_next_field(gll, nullptr);
    nmea_get_next_field(gll, lat);
    nmea_get_next_field(gll, lat_ns);
    nmea_get_next_field(gll, lon);
    nmea_get_next_field(gll, lon_ew);
    nmea_get_next_field(gll, utc);
    nmea_get_next_field(gll, status);
}