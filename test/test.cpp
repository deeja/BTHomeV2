#include <Arduino.h>
#include <unity.h>
#include <bt_home.h>
void setUp()
{
    // set stuff up here
}

void tearDown()
{
    // clean stuff up here
}

void test_packetLength()
{
    // Service data (16-bit UUID): 0A 16 D2FC4002C40903BF13 (BTHome data)
    //
    // 0x0A = length (10 bytes)
    // 0x16 = Service Data - 16-bit UUID
    // 0xD2FC4002C40903BF13 = BTHome data (see below)

    // 020106 = Flags data
    // 04 = Length of the service data (4 bytes)
    // 16 = Service Data - 16-bit UUID
    // D2FC40 = BTHome UUID

    Serial.println("Testing BtHome packet length...");
    BtHome btHome("CountTest", false);
    // 4 bytes
    TEST_ASSERT_EQUAL_STRING("0201060416D2FC40", btHome.getBytes().c_str());
    btHome.addCount_0_255(1);
    // 6 bytes
    TEST_ASSERT_EQUAL_STRING("0201060616D2FC400901", btHome.getBytes().c_str());
    btHome.addCount_0_255(1);
    // 8 bytes 
    TEST_ASSERT_EQUAL_STRING("0201060816D2FC4009010901", btHome.getBytes().c_str());
    btHome.addCount_0_255(1);
}

void test_addTemperature()
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

void test_addDistance()
{
    BtHome btHome("TestDevice", false);
    std::string result = btHome.getBytes();
    TEST_ASSERT_EQUAL_STRING("0201060416D2FC40", result.c_str());
    btHome.addDistanceMetres(7.8);
    TEST_ASSERT_EQUAL_STRING("0201060716D2FC40414E00", btHome.getBytes().c_str());
    btHome.addDistanceMillimetres(12);
    TEST_ASSERT_EQUAL_STRING("0201060A16D2FC40414E00400C00", btHome.getBytes().c_str());
}

void test_addCount()
{
    BtHome btHome("TestDevice", false);
    std::string result = btHome.getBytes();
    TEST_ASSERT_EQUAL_STRING("0201060416D2FC40", result.c_str());
    bool success = btHome.addCount_0_255(96);
    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL_STRING("0201060416D2FC400960", result.c_str());

    btHome.addCount_0_65535(24585);
    TEST_ASSERT_EQUAL_STRING("0201060416D2FC4009603D0960", result.c_str());

    btHome.addCount_0_4294967295(1611213866);
    TEST_ASSERT_EQUAL_STRING("0201060416D2FC4009603D09603E2A2C0960", result.c_str());

    btHome.addCount_neg128_127(-22);
    TEST_ASSERT_EQUAL_STRING("0201060416D2FC4009603D09603E2A2C096059EA", result.c_str());


    btHome.addCount_neg32768_32767(-5398);
    TEST_ASSERT_EQUAL_STRING("0201060416D2FC4009603D09603E2A2C096059EA5AEAEA", result.c_str());

    btHome.addCount_neg2147483648_2147483647(-365690134);
    TEST_ASSERT_EQUAL_STRING("0201060416D2FC4009603D09603E2A2C096059EA5AEAEA5BEA0234EA", result.c_str());
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
    RUN_TEST(test_packetLength);
    RUN_TEST(test_addTemperature);
    RUN_TEST(test_addDistance);
    RUN_TEST(test_addCount);

    delay(500);
    UNITY_END(); // stop unit testing
}