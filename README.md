# ESP32 Binary Packet Serial

Uma biblioteca para comunicação serial binária para ESP32 usando pacotes estruturados. Esta implementação fornece um protocolo de comunicação robusto e eficiente para troca de dados entre dispositivos.

## Características

- Protocolo binário eficiente
- Suporte a múltiplos tipos de pacotes
- Sistema de handlers para processamento de eventos
- Verificação de integridade dos dados
- Buffer de tamanho configurável
- Suporte para dados de sensores e comandos

## Estrutura do Projeto

```plaintext
lib/
  ├── Dispatcher/      # Gerenciamento de handlers de eventos
  ├── Packet/         # Definição da estrutura de pacotes
  ├── Packetizer/     # Serialização de pacotes
  ├── Parser/         # Análise de pacotes recebidos
  ├── PayloadHelper/  # Auxiliar para manipulação de payload
  └── SerialProtocol/ # Interface principal do protocolo
src/
  └── main.cpp        # Exemplo de implementação
test/
  ├── test_communication/    # Testes de comunicação
  └── test_payload_helper/   # Testes do PayloadHelper
```

## Protocolo

O protocolo utiliza os seguintes elementos:

- Byte de início (START_BYTE): `0x7E`
- Tamanho máximo do payload: 250 bytes
- Estrutura do pacote:
  - Tipo (1 byte)
  - Comprimento (1 byte)
  - Payload (até 250 bytes)

### Tipos de Pacotes Implementados

1. `TYPE_SENSOR_DATA (1)`: Dados de sensores
2. `TYPE_COMMAND (2)`: Comandos
3. `TYPE_ACKNOWLEDGE (3)`: Confirmação

## Uso

### Inicialização

```cpp
#include "SerialProtocol.h"

// Cria instância usando Serial2 do ESP32
SerialProtocol protocol(Serial2);

// Inicializa com baud rate desejado
protocol.begin(9600);
```

### Registrando Handlers

```cpp
// Registra handler para pacotes de dados de sensor
protocol.onEvent(TYPE_SENSOR_DATA, handleSensorData);

// Registra handler para pacotes de comando
protocol.onEvent(TYPE_COMMAND, handleCommand);
```

### Enviando Pacotes

```cpp
// Exemplo: enviando dados de sensor
uint8_t sensorPayload[4];
PayloadHelper payloadHelper(sensorPayload);
payloadHelper.compile(temperature, humidity);

protocol.sendPacket(TYPE_SENSOR_DATA, sensorPayload, sizeof(sensorPayload));
```

### Processamento de Pacotes

No loop principal:

```cpp
void loop() {
    protocol.update();
    // ... resto do código
}
```

## Dependências

- [CRC Library](https://github.com/RobTillaart/CRC.git)
- Unity (para testes unitários)

## Instalação

Este é um projeto PlatformIO. Para instalar:

1. Clone o repositório
2. Abra o projeto no PlatformIO
3. As dependências serão instaladas automaticamente
4. Compile e faça upload para seu ESP32

## Testes

O projeto inclui testes unitários que podem ser executados através do PlatformIO:

```bash
pio test
```

## TODOs

- [ ] Implementar reconhecimento do ACK no teste com MockSerial:
  - Verificar se o ACK é recebido corretamente após envio dos dados do sensor.
  - Validar o payload do ACK.
  - Adicionar assertions para garantir que o ACK foi processado.

## Licença

Este projeto está sob licença MIT.

---

Desenvolvido para ESP32 usando PlatformIO e framework Arduino.
