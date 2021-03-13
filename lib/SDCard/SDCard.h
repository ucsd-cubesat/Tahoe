#ifndef SDCARD_H
#define SDCARD_H

#include "SPIDevice.h"

#define CMD(n) (0b01000000 | n)

/**
 * Enumerates the many SD commands
 */
enum CMD: uint8_t {
    CMD_GO_IDLE_STATE         = CMD(0),
    CMD_SEND_OP_COND          = CMD(1),
    CMD_SET_BLOCKLEN          = CMD(16),
    CMD_WRITE_BLOCK           = CMD(24),
    CMD_WRITE_MULTIPLE_BLOCK  = CMD(26)
};

/**
 * A structure for building SD commands
 */
typedef struct {
    CMD cmd = CMD_GO_IDLE_STATE;
    uint8_t arg[4] {0, 0, 0, 0};
    uint8_t crc = 1;
    uint8_t ncr;
    uint8_t rsp[5];
} cmd_t;

class SDCard {
public:

    /**
     * @brief Constructs the SDCard driver.
     * @param ss_pin The CS/SS pin on the SD Card
     */
    SDCard(pin_size_t ss_pin) :
        device(SPISettings(1E6, MSBFIRST, 0), ss_pin) {}

    /**
     * @brief Sends a command to the SD Card, storing the response back into the
     *        structure.
     * @param cmd A reference to a command
     */
    void sendCommand(cmd_t &cmd);

private:
    SPIDevice device;
};

#endif /* SDCARD_H */