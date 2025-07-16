#include "Dispatcher.h"

void Dispatcher::onEvent(uint8_t type, Handler handler) {
    handlers[type] = handler;
}

void Dispatcher::dispatch(const Packet &packet) {
    auto it = handlers.find(packet.type);
    
    if (it != handlers.end()) {
        // Chama o handler registrado com o payload e length
        it->second(packet.payload, packet.length);
    }
    // Se não houver handler, o pacote é ignorado silenciosamente
}