#ifndef PARSER_H
#define PARSER_H

#include "Packet.h"
#include "CRC16.h"

/**
 * Classe responsável por analisar bytes recebidos e construir pacotes
 */
class Parser {
public:
    Parser();
    
    // Reseta o parser para o estado inicial
    void reset();
    
    // Processa um byte recebido e atualiza o estado do parser
    // Retorna o resultado do parsing
    ParseResult parseByte(uint8_t byte, Packet &outPacket);
    
private:
    enum class State {
        WAIT_START,   // Esperando byte de início
        WAIT_TYPE,    // Esperando tipo
        WAIT_LENGTH,  // Esperando length
        WAIT_PAYLOAD, // Recebendo payload
        WAIT_CRC1,    // Esperando primeiro byte do CRC
        WAIT_CRC2     // Esperando segundo byte do CRC
    };
    
    State _state;         // Estado atual do parser
    Packet _currentPacket; // Pacote em construção
    uint16_t _calculatedCrc; // CRC calculado
    uint16_t _receivedCrc;   // CRC recebido
    uint8_t _payloadIndex;   // Índice atual no payload
    CRC16 _crc;            // Objeto CRC
};

#endif // PARSER_H