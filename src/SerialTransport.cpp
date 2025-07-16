#include "SerialTransport.h"

SerialTransport::SerialTransport(HardwareSerial& serial) : _serial(serial) {}

void SerialTransport::begin(unsigned long baud) {
    _serial.begin(baud);
}

size_t SerialTransport::write(const uint8_t *buf, size_t len) {
    return _serial.write(buf, len);
}

int SerialTransport::readByte() {
    if (_serial.available()) {
        return _serial.read();
    }
    return -1;
}

int SerialTransport::available() {
    return _serial.available();
}