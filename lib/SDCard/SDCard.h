#ifndef SDCARD_H
#define SDCARD_H

#include <Arduino.h>
#include <SPI.h>

/*
 https://openlabpro.com/guide/interfacing-microcontrollers-with-sd-card/
 http://rjhcoding.com/avrc-sd-interface-2.php
*/

/**
 * Takes a command number and creates a command value
 */
#define CMD(n) (0b01000000 | n)

/**
 * Enumerates the many standard SD commands
 */
enum CMD: uint8_t {
    CMD_GO_IDLE_STATE = CMD(0),
    CMD_SEND_OP_COND  = CMD(8),
    CMD_SEND_STATUS   = CMD(13),
    CMD_SET_BLOCKLEN  = CMD(16),
    CMD_READ_BLOCK    = CMD(17),
    CMD_WRITE_BLOCK   = CMD(24),
    CMD_APP_SPECIFIC  = CMD(55)
};
/**
 * Enumerates the many application-specific SD commands
 */
enum ACMD: uint8_t {
    ACMD_SEND_OP_COND = CMD(41)
};

class SDCard {
public:

    /**
     * @brief Constructs the SDCard driver.
     * @param ss_pin The CS/SS pin on the SD Card
     */
    SDCard(pin_size_t ss_pin) :
        m_settings(250000, MSBFIRST, SPI_MODE0),
        m_ss_pin(ss_pin),
        m_block_len(512) {
        pinMode(m_ss_pin, OUTPUT);
        digitalWrite(m_ss_pin, HIGH);
    }

    /**
     * @brief Initializes the SD card to be ready
     * @return true if successful, false otherwise
     */
    bool init();

    /**
     * @brief Sets the block length for block reads and writes.
     * @param blockLength The length for a block
     * @return true if successful, false otherwise
     */
    bool setBlockLength(uint32_t blockLength);

    /**
     * @brief Reads a whole block of data are stores it into a buffer
     * @param block The block address
     * @param dst   The buffer to recieve data
     */
    bool read(uint32_t block, void* dst);

    /**
     * @brief Reads data from a block and stores it into a buffer
     * @param block  The block address
     * @param offset The offset into the block
     * @param len    The number of bytes to read
     * @param dst    The buffer to recieve data
     * @return true if successful, false otherwise
     */
    bool read(uint32_t block, uint16_t offset, uint16_t len, void* dst);

    /**
     * @brief Writes data into a block from a buffer
     * @param block  The block address
     * @param src    The buffer containing data
     * @return true if successful, false otherwise
     */
    bool write(uint32_t block, void* src);

private:
    SPISettings m_settings;
    pin_size_t m_ss_pin;
    uint32_t m_block_len;

    /**
     * @brief Starts an SPI command. Must be called before sending a command.
     */
    void beginCommand();

    /**
     * @brief Sends a command to the SD Card
     * @param cmd  The command
     * @param args The command arguments
     * @return The device response
     */
    uint8_t sendCommand(CMD cmd, uint32_t args);

    /**
     * @brief Sends an application-specific command to the SD Card
     * @param acmd The command
     * @param args The command arguments
     * @return The device response
     */
    uint8_t sendCommand(ACMD acmd, uint32_t args);

    /**
     * @brief Sends a raw command to the SD Card, without sanity checking
     * @param cmd The command
     * @param the command arguments
     * @return The device response
     */
    uint8_t sendCommandRaw(uint8_t cmd, uint32_t args);

    /**
     * @brief Ends an SPI command. Must be called after sending a command.
     */
    void endCommand();
};

#endif /* SDCARD_H */