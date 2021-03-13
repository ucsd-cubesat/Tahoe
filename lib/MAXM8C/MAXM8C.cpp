#include "MAXM8C.h"
#include "NMEA.h"

// The index to the end of the buffer
#define BUFFER_END (sizeof(m_buffer) - 1)

// The current length of the buffer including the null-terminator, in bytes
#define CURR_LENGTH (m_offset + 1)

void MAXM8C::reset() {
    m_offset = 0;
    m_ready = false;
    m_buffer[0] = 0;
}

void MAXM8C::update(Stream &uart) {
    while (uart.available()) {
        char c = uart.read();
        switch (c) {

        case '\n':
            break;

        case '\r':
            m_ready = nmea_verify_checksum(m_buffer);
            break;

        case '$':
            reset();
            break;

        default:
            if (m_offset < BUFFER_END) {
                m_buffer[m_offset] = c;
                m_offset++;
                m_buffer[m_offset] = 0;
            }
            break;
        }
        // break immediately if sentence is ready
        if (m_ready) {
            break;
        }
    }
}

size_t MAXM8C::available() {
    return m_ready ? m_offset : 0;
}

size_t MAXM8C::read(char *nmea, size_t len) {
    if (!m_ready) {
        return 0;
    }

    // take the smaller of the sizes
    if (len > CURR_LENGTH) {
        len = CURR_LENGTH;
    }

    // copy up to null
    memcpy(nmea, m_buffer, len);

    reset();
    return len;
}