#include "SDCard.h"

bool SDCard::init() {
    uint32_t arg;
    uint8_t status;

    // must supply min of 74 clock cycles with CS high.
    SPI.beginTransaction(m_settings);
    for (uint8_t i = 0; i < 10; i++) {
      SPI.transfer(0XFF);
    }
    SPI.endTransaction();

    beginCommand();

    // command to go idle in SPI mode
    status = sendCommand(CMD_GO_IDLE_STATE, 0);
    if (status != 0x01) goto fail;

    // check SD version
    status = sendCommand(CMD_SEND_OP_COND, 0x1AA);
    if (status & 0X04) goto fail;

    for (int i = 0; i < 4; i++) status = SPI.transfer(0xFF);
    if (status != 0xAA) goto fail;

    // Initialize
    while (sendCommand(ACMD_SEND_OP_COND, 0x40000000) != 0x00);

    endCommand();
    return true;

fail:
    endCommand();
    return false;
}

void SDCard::beginCommand() {
    digitalWrite(m_ss_pin, LOW);
    SPI.beginTransaction(m_settings);
}

void SDCard::endCommand() {
    SPI.endTransaction();
    digitalWrite(m_ss_pin, HIGH);
}

uint8_t SDCard::sendCommand(CMD cmd, uint32_t args) {
    return sendCommandRaw(cmd, args);
}

uint8_t SDCard::sendCommand(ACMD acmd, uint32_t args) {
    sendCommandRaw(CMD_APP_SPECIFIC, 0);
    return sendCommandRaw(acmd, args);
}

uint8_t SDCard::sendCommandRaw(uint8_t cmd, uint32_t args) {

    // wait for not-busy
    while (SPI.transfer(0xFF) != 0xFF);

    // Sends the command part
    SPI.transfer(cmd);
    SPI.transfer(args >> 24);
    SPI.transfer(args >> 16);
    SPI.transfer(args >> 8);
    SPI.transfer(args);

    // Sends CRC
    if (cmd == CMD_GO_IDLE_STATE) {
        SPI.transfer(0x95);
    }
    else if (cmd == CMD_SEND_OP_COND) {
        SPI.transfer(0x87);
    }
    else {
        SPI.transfer(0xFF);
    }

    // Waits for a response to begin
    // All responses have 0 in the LSB
    uint8_t resp;
    uint8_t i = 0;
    do {
      resp = SPI.transfer(0xFF);
    } while ((resp & 0x80) && ++i != 0xFF);

    return resp;
}

bool SDCard::setBlockLength(uint32_t blockLength) {
    beginCommand();
    uint8_t resp = sendCommand(CMD_SET_BLOCKLEN, blockLength);
    endCommand();
    if (resp == 0) {
        m_block_len = blockLength;
        return true;
    }
    return false;
}

bool SDCard::read(uint32_t block, void* dst) {
    return read(block, 0, m_block_len, dst);
}

bool SDCard::read(uint32_t block, uint16_t offset, uint16_t len, void* dst) {
    uint8_t i;
    if (len == 0) return true;
    if ((offset + len) > m_block_len) return false;

    beginCommand();
    uint8_t resp = sendCommand(CMD_READ_BLOCK, block);
    if (resp != 0x00) {
        //Serial.print("SDCard::read error: bad response: ");
        goto fail;
    }

    // waits for start of data
    i = 0;
    do {
        resp = SPI.transfer(0xFF);
        i++;
    } while (i < 0xFF && resp != 0xFE);
    if (i == 0xFF && resp != 0xFE) {
        //Serial.print("SDCard::read error: timeout: ");
        goto fail;
    }

    // skips offset bytes
    for ( ; offset > 0; offset--) SPI.transfer(0xFF);

    // reads len bytes to dst
    SPI.transfer(dst, len);

    endCommand();
    return true;

fail:
    endCommand();
    //Serial.println(resp, 16);
    return false;
}

bool SDCard::write(uint32_t block, void* src) {
    beginCommand();
    uint8_t resp = sendCommand(CMD_WRITE_BLOCK, block);
    if (resp == 0x00) goto fail;

    // write bytes
    SPI.transfer(0xFE);
    SPI.transfer(src, m_block_len);
    SPI.transfer16(0xFFFF);

    resp = SPI.transfer(0xFF);

    endCommand();
    return resp == 0;

fail:
    endCommand();
    return false;
}