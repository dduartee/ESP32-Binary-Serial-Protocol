#include "CRC16.h"

// CRC-16/XMODEM com polinômio 0x1021
uint16_t CRC16::calculate(const uint8_t *data, size_t length) {
    CRC16 crc;
    crc.init();
    crc.update(data, length);
    return crc.getValue();
}

void CRC16::init() {
    _crc = 0;
}

void CRC16::update(const uint8_t *data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        update(data[i]);
    }
}

void CRC16::update(uint8_t data) {
    _crc ^= (uint16_t)data << 8;
    for (int i = 0; i < 8; i++) {
        if (_crc & 0x8000) {
            _crc = (_crc << 1) ^ 0x1021;
        } else {
            _crc <<= 1;
        }
    }
}

uint16_t CRC16::getValue() const {
    return _crc;
}