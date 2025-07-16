#include <HardwareSerial.h>  // adicione caso não esteja vindo transitivamente

#include <vector>

class MockSerial : public HardwareSerial {
 public:
  // chame o construtor de HardwareSerial passando o UART_NUM (0,1 ou 2)
  MockSerial() : HardwareSerial(2) {}

  // NÃO reconfigura o hardware real
  void begin(unsigned long) { /* no-op */ }

  std::vector<uint8_t> rxBuffer;
  std::vector<uint8_t> txBuffer;
  // simula o loopback: copia tudo de txBuffer pra rxBuffer e limpa txBuffer
  int available() override { return (int)rxBuffer.size(); }

  int read() override {
    int b = rxBuffer.front();
    rxBuffer.erase(rxBuffer.begin());
    return b;
  }

  size_t write(const uint8_t* data, size_t len) override {
    txBuffer.insert(txBuffer.end(), data, data + len);
    return len;
  }
};