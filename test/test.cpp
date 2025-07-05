#include <Arduino.h>
#include <unity.h>
#include <BtHomeV2Device.h>
void setUp()
{
    // set stuff up here
}

void tearDown()
{
    // clean stuff up here
}
String getHexString(BtHomeV2Device &btHome)
{
    uint8_t buffer[50];
    size_t size = btHome.getAdvertisementData(buffer);

    String hexStr = "";
    for (size_t i = 0; i < size; ++i)
    {
        if (buffer[i] < 16)
            hexStr += "0";
        hexStr += String(buffer[i], HEX);
    }
    hexStr.toUpperCase();
    TEST_ASSERT_LESS_OR_EQUAL_MESSAGE(MAX_ADVERTISEMENT_SIZE, size, "Should be less than max advertisement size");
    return hexStr;
}

void test_completeName()
{
    TEST_MESSAGE("Testing complete name...");
    BtHomeV2Device btHome("DIY-sensor", "DIY-sensor", false);

    TEST_ASSERT_EQUAL_STRING("0201060B094449592D73656E736F720416D2FC40", getHexString(btHome).c_str());

    TEST_MESSAGE("Adding unexpected test name. Should not change the data");
    BtHomeV2Device btHome2("test", "DIY-sensor", false);
    TEST_ASSERT_EQUAL_STRING("0201060B094449592D73656E736F720416D2FC40", getHexString(btHome2).c_str());
}

void test_completeNameLengthReduction()
{
    Serial.println("Testing BtHome full local name...");
    const char *shortName = "bbb";
    const char *completeName = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaa"; // 30 characters
    BtHomeV2Device btHome(shortName, completeName, false);      // name is too long
    TEST_ASSERT_EQUAL_STRING("020106150961616161616161616161616161616161616161610416D2FC40", getHexString(btHome).c_str());
}

void test_completeNameLengthSwitchover()
{
    Serial.println("Testing BtHome full local name...");
    BtHomeV2Device btHome("bbb", "aaaaaaaaaaaa", false);
    TEST_ASSERT_EQUAL_STRING("0201060D0961616161616161616161616104086262620416D2FC40", getHexString(btHome).c_str());
    btHome.addCount_0_4294967295(1234567890);
    btHome.addCount_0_4294967295(1234567890);
    TEST_ASSERT_EQUAL_STRING("02010604086262620916D2FC403ED2029649", getHexString(btHome).c_str());
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
    BtHomeV2Device btHome("sss", "lllllllllllllllll", false);
    // 4 bytes
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0416D2FC40", getHexString(btHome).c_str());
    btHome.addCount_0_255(1);
    // 6 bytes
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0616D2FC400901", getHexString(btHome).c_str());
    btHome.addCount_0_255(1);
    // 8 bytes
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0816D2FC4009010901", getHexString(btHome).c_str());
    btHome.addCount_0_255(1);
}

void test_addTemperature()
{
    Serial.println("Testing BtHome getBytes method...");
    BtHomeV2Device btHome("sss", "lllllllllllllllll", false);
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0416D2FC40", getHexString(btHome).c_str());
    btHome.addTemperature_neg127_to_127_Resolution_1(-22.0f);
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0616D2FC4057EA", getHexString(btHome).c_str());
    btHome.addTemperature_neg44_to_44_Resolution_0_35(-7.7f);
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0816D2FC4057EA58EA", getHexString(btHome).c_str());
    btHome.addTemperature_neg327_to_327_Resolution_0_01(25.06f);
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0B16D2FC4057EA58EA02CA09", getHexString(btHome).c_str());
    btHome.addTemperature_neg3276_to_3276_Resolution_0_1(27.3f);
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0E16D2FC4057EA58EA02CA09451101", getHexString(btHome).c_str());
}

void test_addDistance()
{
    BtHomeV2Device btHome("sss", "lllllllllllllllll", false);
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0416D2FC40", getHexString(btHome).c_str());
    btHome.addDistanceMetres(7.8);
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0716D2FC40414E00", getHexString(btHome).c_str());
    btHome.addDistanceMillimetres(12);
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0A16D2FC40414E00400C00", getHexString(btHome).c_str());
}

void test_addHumidity()
{
    BtHomeV2Device btHome("sss", "lllllllllllllllll", false);
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0416D2FC40", getHexString(btHome).c_str());
    btHome.addHumidityPercent_Resolution_0_01(50.55f); // 41.4% humidity
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0716D2FC4003BF13", getHexString(btHome).c_str());
    btHome.addHumidityPercent_Resolution_1(35.0f); // 41.4% humidity

    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0916D2FC4003BF132E23", getHexString(btHome).c_str());
}

void test_addCount_unsigned_integer()
{
    BtHomeV2Device btHome("sss", "lllllllllllllllll", false);

    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0416D2FC40", getHexString(btHome).c_str());
    bool success = btHome.addCount_0_255(96);
    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0616D2FC400960", getHexString(btHome).c_str());

    btHome.addCount_0_65535(24585);
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0916D2FC4009603D0960", getHexString(btHome).c_str());

    btHome.addCount_0_4294967295(1611213866);
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0E16D2FC4009603D09603E2A2C0960", getHexString(btHome).c_str());
}

void test_addCount_signed_integer()
{
    BtHomeV2Device btHome("sss", "lllllllllllllllll", false);
    btHome.addCount_neg128_127(-22);
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0616D2FC4059EA", getHexString(btHome).c_str());

    btHome.addCount_neg32768_32767(-5398);
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0916D2FC4059EA5AEAEA", getHexString(btHome).c_str());

    btHome.addCount_neg2147483648_2147483647(-365690134);
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0E16D2FC4059EA5AEAEA5BEA0234EA", getHexString(btHome).c_str());
}

void test_state_battery()
{
    // 0x15 	battery 	uint8 (1 byte) 	1501 	0 (False = Normal) 1 (True = Low)
    BtHomeV2Device btHome("sss", "lllllllllllllllll", false);
    btHome.setBatteryState(BATTERY_STATE_NORMAL);
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0616D2FC401500", getHexString(btHome).c_str());

    BtHomeV2Device btHome2("sss", "lllllllllllllllll", false);
    btHome2.setBatteryState(BATTERY_STATE_LOW);
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0616D2FC401501", getHexString(btHome2).c_str());
}

void test_addText()
{
    BtHomeV2Device btHome("sss", "lllllllllllllllll", false);
    btHome.addText("Hello World!");

    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C1216D2FC40530C48656C6C6F20576F726C6421", getHexString(btHome).c_str());
}

void test_addTime()
{
    BtHomeV2Device btHome("sss", "lllllllllllllllll", false);

    uint64_t secondsSinceEpoch = 1684093277;
    btHome.addTime(secondsSinceEpoch);

    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0916D2FC40505D396164", getHexString(btHome).c_str());
}

void test_addRaw()
{
    BtHomeV2Device btHome("sss", "lllllllllllllllll", false);
    uint8_t array[4] = {0x00, 0x01, 0x02, 0x03};
    btHome.addRaw(array, 4);

    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0A16D2FC40540400010203", getHexString(btHome).c_str());
}

void test_eventButton()
{
    BtHomeV2Device btHome("sss", "lllllllllllllllll", false);
    btHome.setButtonEvent(Button_Event_Status_Double_Press);

    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0616D2FC403A02", getHexString(btHome).c_str());
    btHome.setButtonEvent(Button_Event_Status_Long_Triple_Press);

    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0816D2FC403A023A06", getHexString(btHome).c_str());
}

void test_eventDimmer()
{
    BtHomeV2Device btHome("sss", "lllllllllllllllll", false);
    btHome.setDimmerEvent(Dimmer_Event_Status_RotateLeft, 3);

    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0716D2FC403C0103", getHexString(btHome).c_str());
    btHome.setDimmerEvent(Dimmer_Event_Status_None, 120);
    TEST_ASSERT_EQUAL_STRING("02010605096C6C6C6C0A16D2FC403C01033C0078", getHexString(btHome).c_str());
}

void test_maxPacketSize()
{
    BtHomeV2Device btHome("sss", "lllllllllllllllll", false);

    uint8_t advertisementData[MAX_ADVERTISEMENT_SIZE];

    for (size_t i = 0; i < 50; i++)
    {
        btHome.setColdState(Cold_Sensor_Status_Cold);
        uint8_t size = btHome.getAdvertisementData(advertisementData);
        char msg[32];
        snprintf(msg, sizeof(msg), "Iteration %zu, size: %u", i, size);
        TEST_ASSERT_LESS_OR_EQUAL_MESSAGE(MAX_ADVERTISEMENT_SIZE, size, msg);
    }
}

void test_shortAndCompleteNames()
{
    // under limits
    BtHomeV2Device btHome("aaaaaaaaa", "lllllllll", false);
    TEST_ASSERT_EQUAL_STRING("0201060A096C6C6C6C6C6C6C6C6C0A086161616161616161610416D2FC40", getHexString(btHome).c_str());

    // on limits
    BtHomeV2Device btHome2("aaa", "ffffffffffffffffffff", false);
    TEST_ASSERT_EQUAL_STRING("0201061509666666666666666666666666666666666666666604086161610416D2FC40", getHexString(btHome2).c_str());
    BtHomeV2Device btHome2b("aaaaaaaaaa", "ffffffff", false);
    TEST_ASSERT_EQUAL_STRING("020106090966666666666666660B08616161616161616161610416D2FC40", getHexString(btHome2b).c_str());

    // over limits
    BtHomeV2Device btHome3("aaa", "ffffffffffffffffffffffff", false);
    TEST_ASSERT_EQUAL_STRING("0201061509666666666666666666666666666666666666666604086161610416D2FC40", getHexString(btHome3).c_str());
    BtHomeV2Device btHome3b("aaaaaaaaaaaaaaa", "ffffffff", false);
    TEST_ASSERT_EQUAL_STRING("020106090966666666666666660B08616161616161616161610416D2FC40", getHexString(btHome3b).c_str());
}

void test_dataLengthExceeded()
{
    BtHomeV2Device btHome("short", "My complete device name", false);
    uint8_t advertisementData[50];
    size_t size = 0;

    size = btHome.getAdvertisementData(advertisementData);

    TEST_ASSERT_LESS_THAN(32, size);
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
    delay(500);
    UNITY_END(); // stop unit testing
}