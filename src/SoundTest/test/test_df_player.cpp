#include <unity.h>
#include <ArduinoFake.h>
#include <DFPlayerMini.hpp>
#include <MemoryStream.hpp>

// Declare MemoryStream.
MemoryStream stream;

void setUp(void)
{
    ArduinoFakeReset();
    fakeit::When(Method(ArduinoFake(), delay)).AlwaysReturn();
    fakeit::When(OverloadedMethod(ArduinoFake(Serial), print, size_t(char))).AlwaysReturn(1);
    fakeit::When(OverloadedMethod(ArduinoFake(Serial), print, size_t(const __FlashStringHelper *))).AlwaysReturn(1);
    fakeit::When(OverloadedMethod(ArduinoFake(Serial), print, size_t(int, int))).AlwaysReturn(1);
    fakeit::When(OverloadedMethod(ArduinoFake(Serial), println, size_t(void))).AlwaysReturn(1);
}

void tearDown(void) {
    // clean stuff up here
}

void test_wrong_message()
{
    DFPlayerMini player;
    player.begin(&stream);

    uint8_t sendBuffer[DFPLAYER_RECEIVED_LENGTH];
    sendBuffer[0] = MessageStart;
    sendBuffer[1] = MessageVersion;
    sendBuffer[2] = 0x06; //Length
    sendBuffer[3] = MessageEnd;
    stream.write((const uint8_t*)sendBuffer, 4);

    auto result = player.available();

    TEST_ASSERT_FALSE(result);
}

void test_send_reset()
{
    DFPlayerMini player;
    player.begin(&stream);

    player.reset();

    auto test = stream.read();
    TEST_ASSERT_EQUAL_INT8(MessageStart,test);
    test = stream.read();
    TEST_ASSERT_EQUAL_INT8(MessageVersion,test);
    test = stream.read();
    TEST_ASSERT_EQUAL_INT8(0x06,test);
    test = stream.read();
    TEST_ASSERT_EQUAL_INT8(0x0C,test);

    stream.clear();
    
    uint8_t sendBuffer[DFPLAYER_RECEIVED_LENGTH];
    sendBuffer[0] = MessageStart;
    sendBuffer[1] = MessageVersion;
    sendBuffer[2] = 0x06; //Length
    sendBuffer[3] = MessageEnd;
    stream.write((const uint8_t*)sendBuffer, 2);
    
    auto result = player.available();

    TEST_ASSERT_FALSE(result);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_wrong_message);
    RUN_TEST(test_send_reset);

    return UNITY_END();
}