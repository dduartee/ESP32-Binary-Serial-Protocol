#include "SerialProtocol.h"

SerialProtocol::SerialProtocol(HardwareSerial& serial)
    : serial(serial)
{
}

void SerialProtocol::begin(unsigned long baud) {
    serial.begin(baud);
    parser.reset();
}

void SerialProtocol::onEvent(uint8_t type, Handler handler) {
    dispatcher.onEvent(type, handler);
}

bool SerialProtocol::sendPacket(uint8_t type, const uint8_t* payload, uint8_t length) {
    uint16_t packetLength = 0;
    
    if (packetizer.buildPacket(type, payload, length, txBuffer, packetLength)) {
        // Envia o pacote diretamente pela serial
        return serial.write(txBuffer, packetLength) == packetLength;
    }
    
    return false;
}

void SerialProtocol::update() {
    // Processa bytes disponíveis na serial
    while (serial.available() > 0) {
        uint8_t data = serial.read();
        
        Packet packet;
        ParseResult result = parser.parseByte(data, packet);
        
        switch (result) {
            case ParseResult::COMPLETE:
                // Pacote completo e válido, despacha para o handler
                dispatcher.dispatch(packet);
                break;
                
            case ParseResult::CRC_ERROR:
                // Implementar tratamento de erro CRC se necessário
                break;
                
            case ParseResult::SYNC_ERROR:
                // Implementar tratamento de erro de sincronização se necessário
                break;
                
            case ParseResult::NONE:
                // Continua recebendo bytes
                break;
        }
    }
}