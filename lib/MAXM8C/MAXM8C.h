#ifndef MAXM8C_H
#define MAXM8C_H

#include <Arduino.h>

class MAXM8C {
public:

    /**
     * @brief Constructs the device driver.
     */
    MAXM8C() {
        reset();
    };

    /**
     * @brief Processess the current stream of UART information. This is like
     * loop() in  main.c.
     */
    void update();

    /**
     * @brief The number of bytes in the current sentence
     * @return The number of bytes in the current sentence, not including a
     *         null-terminator; 0 if no sentence is available
     */
    size_t available();

    /**
     * @brief Reads the next available sentence. If no sentence is available,
     *        the buffer is left intact
     * @param nmea The character buffer that will contain the sentence
     * @param len  The max capacity of the buffer, which should include space
     *             for a null terminator
     * @return The number of bytes stored into the buffer, including a
     *         null-terminator; or 0 if no sentence is available
     */
    size_t read(char *nmea, size_t len);

private:
    char m_buffer[128];
    size_t m_offset;
    bool m_ready;

    /**
     * @brief Resets the state of the reader.
     */
    void reset();
};

#endif /* MAXM8C_H */