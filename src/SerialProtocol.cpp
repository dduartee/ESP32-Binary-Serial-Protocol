#include "SerialProtocol.h"

SerialProtocol::SerialProtocol(TransportLayer& transport)
    : _transport(transport)
{
}

void SerialProtocol::begin(unsigned long baud) {
    _transport.begin(baud);
    _parser.reset();
}

void SerialProtocol::onEvent(uint8_t type, Handler handler) {
    _dispatcher.onEvent(type, handler);
}

bool SerialProtocol::sendPacket(uint8_t type, const uint8_t* payload, uint8_t length) {
    uint16_t packetLength = 0;
    
    if (_packetizer.buildPacket(type, payload, length, _txBuffer, packetLength)) {
        // Envia o pacote pelo transporte
        return _transport.write(_txBuffer, packetLength) == packetLength;
    }
    
    return false;
}

void SerialProtocol::update() {
    // Processa bytes disponíveis
    while (_transport.available() > 0) {
        int data = _transport.readByte();
        
        if (data >= 0) {
            Packet packet;
            ParseResult result = _parser.parseByte(static_cast<uint8_t>(data), packet);
            
            switch (result) {
                case ParseResult::COMPLETE:
                    // Pacote completo e válido, despacha para o handler
                    _dispatcher.dispatch(packet);
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
}