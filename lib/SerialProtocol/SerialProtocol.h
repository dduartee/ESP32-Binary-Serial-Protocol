#ifndef SERIAL_PROTOCOL_H
#define SERIAL_PROTOCOL_H

#include <HardwareSerial.h>
#include "Packet.h"
#include "Packetizer.h"
#include "Parser.h"
#include "Dispatcher.h"

/**
 * Classe que implementa o protocolo serial
 * Trabalha diretamente com HardwareSerial do ESP32
 */
class SerialProtocol {
public:
    // Construtor recebe uma referência ao objeto HardwareSerial a ser usado
    SerialProtocol(HardwareSerial& serial);
    
    // Inicializa o protocolo com a velocidade especificada
    void begin(unsigned long baud);
    
    // Registra handler para um tipo de evento
    void onEvent(uint8_t type, Handler handler);
    
    // Envia um pacote com o tipo e payload especificados
    bool sendPacket(uint8_t type, const uint8_t* payload, uint8_t length);
    
    // Processa bytes recebidos - deve ser chamado no loop principal
    void update();
    
private:
    HardwareSerial& serial;
    Packetizer packetizer;
    Parser parser;
    Dispatcher dispatcher;
    
    // Buffer para montagem de pacotes
    uint8_t txBuffer[MAX_PAYLOAD_SIZE + 10]; // Payload + overhead
};

#endif // SERIAL_PROTOCOL_H