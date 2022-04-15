#ifndef SPIDEVICE_H
#define SPIDEVICE_H

#include <Arduino.h>
#include <SPI.h>

class SPIDevice {
public:

    /**
     * @brief Constructs an SPI device driver.
     * @param settings The settings for use on the bus
     * @param ss_pin   The CS/SS pin for the device 
     */
    SPIDevice(SPISettings settings, uint16_t ss_pin) :
        m_settings(settings),
        m_ss_pin(ss_pin) {
        pinMode(m_ss_pin, OUTPUT);
        digitalWrite(m_ss_pin, HIGH);
    }

    /**
     * @brief Writes data linearly from the buffer to the SPI device.
     *        The response data is stored linearly into buffer.
     * @param reg    The register address
     * @param buffer The buffer
     * @param len    The size of the buffer in bytes
     */
    void exchange(uint8_t reg, void* buffer, size_t len);

    /**
     * @brief Writes data linearly from the buffer to the SPI device.
     *        The buffer is left intact.
     * @param reg    The register address
     * @param buffer The buffer
     * @param len    The size of the buffer in bytes
     */
    void write(uint8_t reg, void* buffer, size_t len);

    /**
     * @brief Writes a byte to the SPI device.
     * @param reg    The register address
     * @param value  The value to write
     * @return The previous value in the register
     */
    uint8_t write(uint8_t reg, uint8_t value);

    /**
     * @brief Reads data linearly into the buffer from the SPI device.
     * @param reg    The register address
     * @param buffer The buffer
     * @param len    The size of the buffer in bytes
     */
    void read(uint8_t reg, void* buffer, size_t len);

    /**
     * @brief Reads a byte from the SPI device.
     * @param reg The register address
     * @return The value in the register
     */
    uint8_t read(uint8_t reg);

    /**
     * @brief Writes data linearly from the buffer to the SPI device.
     *        reg is updated incrementally for each byte in the buffer.
     *        The response data is stored linearly into buffer.
     * @param reg    The register address
     * @param buffer The buffer
     * @param len    The size of the buffer in bytes
     */
    void exchangeAuto(uint8_t reg, void* buffer, size_t len);

    /**
     * @brief Writes data linearly from the buffer to the SPI device.
     *        reg is updated incrementally for each byte in the buffer.
     *        The buffer is left intact.
     * @param reg    The register address
     * @param buffer The buffer
     * @param len    The size of the buffer in bytes
     */
    void writeAuto(uint8_t reg, void* buffer, size_t len);

    /**
     * @brief Reads data linearly into the buffer from the SPI device.
     *        reg is updated incrementally for each byte in the buffer.
     * @param reg    The register address
     * @param buffer The buffer
     * @param len    The size of the buffer in bytes
     */
    void readAuto(uint8_t reg, void* buffer, size_t len);

private:
    SPISettings m_settings;
    uint16_t m_ss_pin;
};

#endif /* SPIDEVICE_H */