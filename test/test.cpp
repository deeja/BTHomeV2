#include "base.h"
#include "temperature.h"
#include "test_encryption.h"
#include "tests.hpp"
#include "deviceTypeId.hpp"
#include "packetId.hpp"
#include "firmware.hpp"

void setUp()
{
    // set stuff up here
}

void tearDown()
{
    // clean stuff up here
}



void setup()
{
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN(); // IMPORTANT LINE!
}

void loop()
{
    delay(500);

    RUN_TEST(test_encryptionSetCounter);
    RUN_TEST(test_encryptionPayload);
    RUN_TEST(test_encryptionCounter);
    RUN_TEST(test_triggerFlag);
    RUN_TEST(test_dataLengthExceeded);
    RUN_TEST(test_shortAndCompleteNames);
    RUN_TEST(test_maxPacketSize);

    RUN_TEST(test_eventDimmer);
    RUN_TEST(test_eventButton);
    RUN_TEST(test_addRaw);

    RUN_TEST(test_addHumidity);
    RUN_TEST(test_completeNameLengthSwitchover);
    RUN_TEST(test_completeName);
    RUN_TEST(test_packetLength);
    RUN_TEST(test_addTemperature);
    RUN_TEST(test_addDistance);
    RUN_TEST(test_addCount_unsigned_integer);
    RUN_TEST(test_addCount_signed_integer);

    RUN_TEST(test_completeNameLengthReduction);
    RUN_TEST(test_state_battery);
    RUN_TEST(test_addText);
    RUN_TEST(test_addTime);

    RUN_TEST(test_addDeviceTypeId);
    RUN_TEST(test_addPacketId);
    RUN_TEST(test_addFirmware);

    delay(500);
    UNITY_END(); // stop unit testing
}