#ifndef SERIAL_PROTOCOL_H
#define SERIAL_PROTOCOL_H

#include "TransportLayer.h"
#include "SerialTransport.h"
#include "Packet.h"
#include "Packetizer.h"
#include "Parser.h"
#include "Dispatcher.h"

/**
 * Classe que integra todos os componentes do protocolo serial
 */
class SerialProtocol {
public:
    SerialProtocol(TransportLayer& transport);
    
    // Inicializa o protocolo com a velocidade especificada
    void begin(unsigned long baud);
    
    // Registra handler para um tipo de evento
    void onEvent(uint8_t type, Handler handler);
    
    // Envia um pacote com o tipo e payload especificados
    bool sendPacket(uint8_t type, const uint8_t* payload, uint8_t length);
    
    // Processa bytes recebidos - deve ser chamado no loop principal
    void update();
    
private:
    TransportLayer& _transport;
    Packetizer _packetizer;
    Parser _parser;
    Dispatcher _dispatcher;
    
    // Buffer para montagem de pacotes
    uint8_t _txBuffer[MAX_PAYLOAD_SIZE + 10]; // Payload + overhead
};

#endif // SERIAL_PROTOCOL_H