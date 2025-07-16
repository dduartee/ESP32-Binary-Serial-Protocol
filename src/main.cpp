#include <Arduino.h>
#include "SerialProtocol.h"
#include "PayloadHelper.h"
// Define os tipos de pacotes que serão usados
enum PacketTypes {
    TYPE_SENSOR_DATA = 1,
    TYPE_COMMAND = 2,
    TYPE_ACKNOWLEDGE = 3
};

// Cria o protocolo usando diretamente Serial2 do ESP32
SerialProtocol protocol(Serial2);

// Handler para pacotes de dados de sensor
void handleSensorData(const uint8_t* payload, uint8_t length) {
    if (length >= 4) {
        // Extrai os valores do sensor do payload
        float temperature = (payload[0] << 8 | payload[1]) / 10.0f;
        float humidity = (payload[2] << 8 | payload[3]) / 10.0f;
        
        Serial.print("Temperatura: ");
        Serial.print(temperature);
        Serial.print("°C, Umidade: ");
        Serial.print(humidity);
        Serial.println("%");
        
        // Envia um pacote de reconhecimento
        uint8_t ackPayload = 1;  // 1 = recebido com sucesso
        protocol.sendPacket(TYPE_ACKNOWLEDGE, &ackPayload, 1);
    }
}

// Handler para pacotes de comando
void handleCommand(const uint8_t* payload, uint8_t length) {
    if (length >= 1) {
        uint8_t command = payload[0];
        
        Serial.print("Comando recebido: ");
        Serial.println(command);
        
        // Processa o comando...
        
        // Envia um pacote de reconhecimento
        uint8_t ackPayload = 1;  // 1 = comando executado
        protocol.sendPacket(TYPE_ACKNOWLEDGE, &ackPayload, 1);
    }
}

void setup() {
    // Inicializa a Serial para debugging
    Serial.begin(115200);
    
    // Inicializa o protocolo com baudrate de 9600
    protocol.begin(9600);
    
    // Registra handlers para os tipos de pacotes
    protocol.onEvent(TYPE_SENSOR_DATA, handleSensorData);
    protocol.onEvent(TYPE_COMMAND, handleCommand);
    
    Serial.println("Sistema inicializado!");
}

void loop() {
    // Atualiza o protocolo para processar pacotes recebidos
    protocol.update();
    
    // Exemplo: enviar dados de sensor a cada 10 segundos
    static unsigned long lastSendTime = 0;
    if (millis() - lastSendTime > 10000) {
        lastSendTime = millis();
        
        // Simula dados de sensor (temperatura = 25.5°C, umidade = 65.2%)
        uint8_t sensorPayload[4];
        int16_t temp = 255;   // 25.5°C * 10
        int16_t humid = 652;  // 65.2% * 10
        
        PayloadHelper payloadHelper(sensorPayload);
        payloadHelper.compile(temp, humid);

        // Envia pacote
        if (protocol.sendPacket(TYPE_SENSOR_DATA, sensorPayload, sizeof(sensorPayload))) {
            Serial.println("Dados de sensor enviados!");
        } else {
            Serial.println("Erro ao enviar dados!");
        }
    }
}