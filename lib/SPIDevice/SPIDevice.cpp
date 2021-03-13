#include "SPIDevice.h"

void SPIDevice::exchange(uint8_t reg, void* buffer, size_t len) {
    digitalWrite(m_ss_pin, LOW);
    SPI.beginTransaction(m_settings);

    SPI.transfer(reg);
    SPI.transfer(buffer, len);

    SPI.endTransaction();
    digitalWrite(m_ss_pin, HIGH);
}

void SPIDevice::write(uint8_t reg, void* buffer, size_t len) {
    uint8_t temp[len];
    memcpy(temp, buffer, len);
    exchange(reg & 0x7F, temp, len);
}

uint8_t SPIDevice::write(uint8_t reg, uint8_t value) {
    exchange(reg & 0x7F, &value, 1);
    return value;
}

void SPIDevice::read(uint8_t reg, void* buffer, size_t len) {
    digitalWrite(m_ss_pin, LOW);
    SPI.beginTransaction(m_settings);

    SPI.transfer(reg | 0x80);
    SPI.transfer(buffer, len);

    SPI.endTransaction();
    digitalWrite(m_ss_pin, HIGH);
}

uint8_t SPIDevice::read(uint8_t reg) {
    uint8_t value;
    read(reg, &value, 1);
    return value;
}

void SPIDevice::exchangeAuto(uint8_t reg, void* buffer, size_t len) {
    uint8_t *b = (uint8_t*)buffer;
    SPI.beginTransaction(m_settings);
    for (size_t i = 0; i < len; i++) {
        digitalWrite(m_ss_pin, LOW);

        SPI.transfer(reg);
        b[i] = SPI.transfer(b[i]);
        reg++;

        digitalWrite(m_ss_pin, HIGH);
    }
    SPI.endTransaction();
}

void SPIDevice::writeAuto(uint8_t reg, void* buffer, size_t len) {
    uint8_t *b = (uint8_t*)buffer;
    SPI.beginTransaction(m_settings);
    for (size_t i = 0; i < len; i++) {
        digitalWrite(m_ss_pin, LOW);

        SPI.transfer(reg & 0x7F);
        SPI.transfer(b[i]);
        reg++;

        digitalWrite(m_ss_pin, HIGH);
    }
    SPI.endTransaction();
}

void SPIDevice::readAuto(uint8_t reg, void* buffer, size_t len) {
    uint8_t *b = (uint8_t*)buffer;
    SPI.beginTransaction(m_settings);
    for (size_t i = 0; i < len; i++) {
        digitalWrite(m_ss_pin, LOW);

        SPI.transfer(reg | 0x80);
        b[i] = SPI.transfer(0);
        reg++;

        digitalWrite(m_ss_pin, HIGH);
    }
    SPI.endTransaction();
}