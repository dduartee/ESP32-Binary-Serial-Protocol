#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

// Constantes para o protocolo
constexpr uint8_t START_BYTE = 0x7E;
constexpr uint8_t MAX_PAYLOAD_SIZE = 250;

// Estrutura de um pacote completo
struct Packet {
    uint8_t type;
    uint8_t length;
    uint8_t payload[MAX_PAYLOAD_SIZE];
    
    Packet() : type(0), length(0) {}
};

// Resultados possíveis da análise de pacotes
enum class ParseResult {
    NONE,       // Nenhum pacote completo ainda
    COMPLETE,   // Pacote completo e válido
    CRC_ERROR,  // Erro de CRC
    SYNC_ERROR  // Erro de sincronização
};

#endif // PACKET_H