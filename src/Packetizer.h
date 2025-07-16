#ifndef PACKETIZER_H
#define PACKETIZER_H

#include <stdint.h>
#include <stddef.h>
#include "CRC16.h"
#include "Packet.h"

/**
 * Classe responsável por montar pacotes para envio
 */
class Packetizer {
public:
    // Constrói um pacote completo com start byte, tipo, length, payload e CRC
    // Retorna true se o pacote foi construído com sucesso
    bool buildPacket(uint8_t type, const uint8_t *payload, uint8_t length, 
                     uint8_t *outBuf, uint16_t &outLen);
                     
    // Tamanho total do pacote dado o tamanho do payload
    static uint16_t packetSize(uint8_t payloadLength);
    
private:
    CRC16 _crc;
};

#endif // PACKETIZER_H