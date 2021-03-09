#ifndef MAXM8C_H
#define MAXM8C_H

#include <Arduino.h>

class MAXM8C {
public:

    /**
     * @brief Constructs the device driver.
     */
    MAXM8C() {
        Serial1.begin(9600);
    };

    /**
     * @brief Waits for the next complete NMEA sentence and reads it into the
     *        buffer, null-terminating it and removing delimiters.
     * @param nmea The character buffer that will contain the sentence
     * @param len  The max capacity of the buffer, including space for a null
     * @return 0 if the checksum passes, otherwise the supposed checksum
     */
    uint8_t readSentence(char *nmea, size_t len);
};

#endif /* MAXM8C_H */