#include <Arduino.h>
#include <unity.h>
#include <bt_home.h>
void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

void test_addTemperature(void)
{
    Serial.println("Testing BtHome getBytes method...");
    BtHome btHome("TestDevice", false);
    std::string result = btHome.getBytes();
    TEST_ASSERT_EQUAL_STRING("0201060416D2FC40", result.c_str());
    btHome.addTemperature(-22.0f, RANGE_127_RESOLUTION_1);
    TEST_ASSERT_EQUAL_STRING("0201060616D2FC4057EA", btHome.getBytes().c_str());
    btHome.addTemperature(-7.7f, RANGE_44_RESOLUTION_0_35);
    TEST_ASSERT_EQUAL_STRING("0201060816D2FC4057EA58EA", btHome.getBytes().c_str());
    btHome.addTemperature(25.06f, RANGE_327_RESOLUTION_0_01);
    TEST_ASSERT_EQUAL_STRING("0201060B16D2FC4057EA58EA02CA09", btHome.getBytes().c_str());
    btHome.addTemperature(27.3f, RANGE_3276_RESOLUTION_0_1);
    TEST_ASSERT_EQUAL_STRING("0201060E16D2FC4057EA58EA02CA09451101", btHome.getBytes().c_str());
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
    RUN_TEST(test_addTemperature);
    delay(500);
    UNITY_END(); // stop unit testing
}