// filepath: [PayloadHelper.h](http://_vscodecontentref_/2)
#ifndef PAYLOAD_HELPER_H
#define PAYLOAD_HELPER_H

#include <Arduino.h>

class PayloadHelper {
private:
    uint8_t* _payload;
public:
    explicit PayloadHelper(uint8_t* payload);
    void compile(int16_t temp, int16_t humid);
    void parse(int16_t& temp, int16_t& humid);
};

#endif // PAYLOAD_HELPER_H