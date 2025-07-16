#ifndef SERIAL_TRANSPORT_H
#define SERIAL_TRANSPORT_H

#include "TransportLayer.h"
#include <HardwareSerial.h>

/**
 * Implementação da camada de transporte usando HardwareSerial do ESP32
 */
class SerialTransport : public TransportLayer {
public:
    SerialTransport(HardwareSerial& serial);
    
    void begin(unsigned long baud) override;
    size_t write(const uint8_t *buf, size_t len) override;
    int readByte() override;
    int available() override;
    
private:
    HardwareSerial& _serial;
};

#endif // SERIAL_TRANSPORT_H