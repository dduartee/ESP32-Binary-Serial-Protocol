#include "Packetizer.h"

bool Packetizer::buildPacket(uint8_t type, const uint8_t *payload, uint8_t length, 
                            uint8_t *outBuf, uint16_t &outLen) {
    // Verifica se o payload não excede o tamanho máximo
    if (length > MAX_PAYLOAD_SIZE) {
        return false;
    }
    
    uint16_t index = 0;
    
    // Adiciona o byte de início
    outBuf[index++] = START_BYTE;
    
    // Adiciona tipo e length
    outBuf[index++] = type;
    outBuf[index++] = length;
    
    // Adiciona payload
    for (uint8_t i = 0; i < length; i++) {
        outBuf[index++] = payload[i];
    }
    
    // Calcula CRC sobre tipo, length e payload
    _crc.init();
    _crc.update(type);
    _crc.update(length);
    _crc.update(payload, length);
    
    uint16_t crcValue = _crc.getValue();
    
    // Adiciona CRC (2 bytes - MSB primeiro)
    outBuf[index++] = (crcValue >> 8) & 0xFF;
    outBuf[index++] = crcValue & 0xFF;
    
    // Atualiza tamanho total do pacote
    outLen = index;
    
    return true;
}

uint16_t Packetizer::packetSize(uint8_t payloadLength) {
    // start (1) + type (1) + length (1) + payload (n) + crc (2)
    return 1 + 1 + 1 + payloadLength + 2;
}