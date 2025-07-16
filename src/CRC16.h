#ifndef CRC16_H
#define CRC16_H

#include <stdint.h>
#include <stddef.h>

/**
 * Classe para calcular e verificar CRC-16
 * Usa algoritmo CRC-16/XMODEM: x^16 + x^12 + x^5 + 1 (0x1021)
 */
class CRC16 {
public:
    static uint16_t calculate(const uint8_t *data, size_t length);
    
    // Inicializar com valor padrão
    void init();
    
    // Atualizar CRC com novos bytes
    void update(const uint8_t *data, size_t length);
    void update(uint8_t data);
    
    // Obter valor CRC atual
    uint16_t getValue() const;
    
private:
    uint16_t _crc = 0;
};

#endif // CRC16_H