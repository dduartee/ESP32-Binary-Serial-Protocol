#include "Parser.h"

Parser::Parser() {
    reset();
}

void Parser::reset() {
    _state = State::WAIT_START;
    _payloadIndex = 0;
    _calculatedCrc = 0;
    _receivedCrc = 0;
    _currentPacket = Packet();
}

ParseResult Parser::parseByte(uint8_t byte, Packet &outPacket) {
    switch (_state) {
        case State::WAIT_START:
            if (byte == START_BYTE) {
                reset();
                _state = State::WAIT_TYPE;
            }
            return ParseResult::NONE;
            
        case State::WAIT_TYPE:
            _currentPacket.type = byte;
            _crc.init();
            _crc.update(byte); // Inicia cálculo de CRC com o tipo
            _state = State::WAIT_LENGTH;
            return ParseResult::NONE;
            
        case State::WAIT_LENGTH:
            _currentPacket.length = byte;
            _crc.update(byte); // Adiciona length ao CRC
            
            // Verifica se o payload tem tamanho válido
            if (_currentPacket.length > MAX_PAYLOAD_SIZE) {
                reset();
                return ParseResult::SYNC_ERROR;
            }
            
            // Se não houver payload, vá direto para o CRC
            if (_currentPacket.length == 0) {
                _state = State::WAIT_CRC1;
            } else {
                _state = State::WAIT_PAYLOAD;
                _payloadIndex = 0;
            }
            return ParseResult::NONE;
            
        case State::WAIT_PAYLOAD:
            _currentPacket.payload[_payloadIndex++] = byte;
            _crc.update(byte); // Adiciona byte do payload ao CRC
            
            // Verifica se recebeu todo o payload
            if (_payloadIndex >= _currentPacket.length) {
                _state = State::WAIT_CRC1;
            }
            return ParseResult::NONE;
            
        case State::WAIT_CRC1:
            _receivedCrc = byte << 8; // Primeiro byte do CRC (MSB)
            _state = State::WAIT_CRC2;
            return ParseResult::NONE;
            
        case State::WAIT_CRC2:
            _receivedCrc |= byte; // Segundo byte do CRC (LSB)
            _calculatedCrc = _crc.getValue();
            
            // Prepara para o próximo pacote
            _state = State::WAIT_START;
            
            // Verifica CRC
            if (_calculatedCrc != _receivedCrc) {
                return ParseResult::CRC_ERROR;
            }
            
            // Copia o pacote atual para saída
            outPacket = _currentPacket;
            return ParseResult::COMPLETE;
    }
    
    // Não deveria chegar aqui
    return ParseResult::SYNC_ERROR;
}