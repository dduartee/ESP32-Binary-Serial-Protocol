#include <unity.h>

#include <vector>

#include "MockSerial.h"
#include "Parser.h"
#include "PayloadHelper.h"
#include "SerialProtocol.h"

// mesmo valores do seu main.cpp
#define TYPE_SENSOR_DATA 1
#define TYPE_ACKNOWLEDGE 3

// Dispositivo “sensor” envia pacote de dados
static void test_sensor_data_triggers_ack(void) {
  MockSerial serial;

  SerialProtocol protocolTx(serial);
  SerialProtocol protocolRx(serial);

  protocolTx.begin(9600);
  protocolRx.begin(9600);

  // TODO: Implementar reconhecimento do ACK no teste
  // - Verificar se o ACK é recebido corretamente após envio dos dados do sensor
  // - Validar o payload do ACK
  // - Adicionar assertions para garantir que o ACK foi processado

  // registra handler igual ao handleSensorData do main.cpp
  protocolRx.onEvent(TYPE_SENSOR_DATA,
                     [&](const uint8_t* payload, uint8_t length) {
                    //    if (length >= 4) {
                    //      // envia ACK
                    //      uint8_t ack = 1;
                    //      protocolRx.sendPacket(TYPE_ACKNOWLEDGE, &ack, 1);
                    //    }
                     });

  // payload de exemplo —  assume temp=300 (30.0°C) e humid=400 (40.0%)
  // uint8_t payload[4] = { 0x01, 0x2C, 0x01, 0x90 };
  uint8_t payload[4];
  PayloadHelper payloadHelper(payload);
  payloadHelper.compile(300, 400);  // 30.0°C e 40.0% de umidade
  // sensor envia pacote
  TEST_ASSERT_TRUE(
      protocolTx.sendPacket(TYPE_SENSOR_DATA, payload, sizeof(payload)));

  // receptor processa bytes
  protocolRx.update();

  TEST_ASSERT_GREATER_THAN(0, serial.txBuffer.size());

  // usamos o Parser para extrair o pacote de ACK
  Parser parser;
  parser.reset();
  Packet out;
  for (auto b : serial.txBuffer) {
    if (parser.parseByte(b, out) == ParseResult::COMPLETE) {
      TEST_ASSERT_EQUAL(TYPE_SENSOR_DATA, out.type);
      TEST_ASSERT_EQUAL(sizeof(payload), out.length);
      TEST_ASSERT_EQUAL(1, out.payload[0]);
      return;
    }
  }

  TEST_FAIL_MESSAGE("ACK nao recebido");
}

void setUp(void) {}
void tearDown(void) {}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_sensor_data_triggers_ack);
  UNITY_END();
}

void loop() {}