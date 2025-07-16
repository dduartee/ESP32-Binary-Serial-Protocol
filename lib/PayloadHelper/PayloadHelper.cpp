#include "PayloadHelper.h"

PayloadHelper::PayloadHelper(uint8_t* payload)
  : _payload(payload)
{}

void PayloadHelper::compile(int16_t temp, int16_t humid) {
    // empacota temp (2 bytes) e humid (2 bytes) em big-endian
    _payload[0] = uint8_t((temp >> 8) & 0xFF);
    _payload[1] = uint8_t(temp & 0xFF);
    _payload[2] = uint8_t((humid >> 8) & 0xFF);
    _payload[3] = uint8_t(humid & 0xFF);
}

void PayloadHelper::parse(int16_t& temp, int16_t& humid) {
    // desempacota big-endian para os inteiros
    temp  = int16_t((_payload[0] << 8) | _payload[1]);
    humid = int16_t((_payload[2] << 8) | _payload[3]);
}