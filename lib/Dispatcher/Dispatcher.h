#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "Packet.h"
#include <functional>
#include <map>

// Tipo para callback de eventos
using Handler = std::function<void(const uint8_t*, uint8_t)>;

/**
 * Classe responsável por despachar pacotes para os handlers correspondentes
 */
class Dispatcher {
public:
    // Registra um handler para um tipo específico de pacote
    void onEvent(uint8_t type, Handler handler);
    
    // Despacha um pacote para o handler correspondente
    void dispatch(const Packet &packet);
    
private:
    std::map<uint8_t, Handler> handlers;
};

#endif // DISPATCHER_H