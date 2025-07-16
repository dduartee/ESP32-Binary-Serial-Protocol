#include "Parser.h"

Parser::Parser() {
    reset();
}

void Parser::reset() {
    state = State::WAIT_START;
    payloadIndex = 0;
    calculatedCrc = 0;
    receivedCrc = 0;
    currentPacket = Packet();
}

ParseResult Parser::parseByte(uint8_t byte, Packet &outPacket) {
    switch (state) {
        case State::WAIT_START:
            if (byte == START_BYTE) {
                reset();
                state = State::WAIT_TYPE;
            }
            return ParseResult::NONE;
            
        case State::WAIT_TYPE:
            currentPacket.type = byte;
            // Reinicia cálculo de CRC
            crc.restart();
            crc.add(byte); // Inicia cálculo de CRC com o tipo
            state = State::WAIT_LENGTH;
            return ParseResult::NONE;
            
        case State::WAIT_LENGTH:
            currentPacket.length = byte;
            crc.add(byte); // Adiciona length ao CRC
            
            // Verifica se o payload tem tamanho válido
            if (currentPacket.length > MAX_PAYLOAD_SIZE) {
                reset();
                return ParseResult::SYNC_ERROR;
            }
            
            // Se não houver payload, vá direto para o CRC
            if (currentPacket.length == 0) {
                state = State::WAIT_CRC1;
            } else {
                state = State::WAIT_PAYLOAD;
                payloadIndex = 0;
            }
            return ParseResult::NONE;
            
        case State::WAIT_PAYLOAD:
            currentPacket.payload[payloadIndex++] = byte;
            crc.add(byte); // Adiciona byte do payload ao CRC
            
            // Verifica se recebeu todo o payload
            if (payloadIndex >= currentPacket.length) {
                state = State::WAIT_CRC1;
            }
            return ParseResult::NONE;
            
        case State::WAIT_CRC1:
            receivedCrc = byte << 8; // Primeiro byte do CRC (MSB)
            state = State::WAIT_CRC2;
            return ParseResult::NONE;
            
        case State::WAIT_CRC2:
            receivedCrc |= byte; // Segundo byte do CRC (LSB)
            calculatedCrc = crc.calc();
            
            // Prepara para o próximo pacote
            state = State::WAIT_START;
            
            // Verifica CRC
            if (calculatedCrc != receivedCrc) {
                return ParseResult::CRC_ERROR;
            }
            
            // Copia o pacote atual para saída
            outPacket = currentPacket;
            return ParseResult::COMPLETE;
    }
    
    // Não deveria chegar aqui
    return ParseResult::SYNC_ERROR;
}