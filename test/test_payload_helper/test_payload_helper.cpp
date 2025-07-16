// filepath: test/test_payload_helper.cpp
#include <unity.h>
#include "PayloadHelper.h"

static void test_compile_and_parse(void) {
    // buffer de 4 bytes
    uint8_t buf[4];
    PayloadHelper helper(buf);

    int16_t temp_in  = 300; // 30.0°C
    int16_t humid_in = 400; // 40.0%

    helper.compile(temp_in, humid_in);

    // verifica big-endian bruto
    TEST_ASSERT_EQUAL_UINT8((temp_in >> 8) & 0xFF, buf[0]);
    TEST_ASSERT_EQUAL_UINT8(temp_in & 0xFF,        buf[1]);
    TEST_ASSERT_EQUAL_UINT8((humid_in >> 8) & 0xFF, buf[2]);
    TEST_ASSERT_EQUAL_UINT8(humid_in & 0xFF,        buf[3]);

    // agora o parse
    int16_t temp_out = 0, humid_out = 0;
    helper.parse(temp_out, humid_out);
    TEST_ASSERT_EQUAL_INT16(temp_in,  temp_out);
    TEST_ASSERT_EQUAL_INT16(humid_in, humid_out);
}

void setUp()   {}
void tearDown(){}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_compile_and_parse);
    UNITY_END();
}

void loop() {}