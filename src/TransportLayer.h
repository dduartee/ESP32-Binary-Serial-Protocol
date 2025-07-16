#ifndef TRANSPORT_LAYER_H
#define TRANSPORT_LAYER_H

#include <stdint.h>
#include <stddef.h>

/**
 * Interface abstrata para a camada de transporte
 * Permite implementações para diferentes interfaces físicas (UART, RS-485, etc)
 */
class TransportLayer {
public:
    virtual ~TransportLayer() {}
    
    // Inicializa a camada de transporte
    virtual void begin(unsigned long baud) = 0;
    
    // Escreve um buffer de bytes
    virtual size_t write(const uint8_t *buf, size_t len) = 0;
    
    // Lê um byte, retorna -1 se nada disponível
    virtual int readByte() = 0;
    
    // Verifica quantos bytes estão disponíveis para leitura
    virtual int available() = 0;
};

#endif // TRANSPORT_LAYER_H