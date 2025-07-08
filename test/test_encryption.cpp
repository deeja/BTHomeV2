#include "test_encryption.h"
#include "mbedtls/ccm.h"
#include <NimBLEDevice.h>

void test_encryptionPayload()
{

    const uint8_t mac[] = {0x72, 0x2E, 0x6C, 0x1B, 0x5A, 0xE0 };

    const uint8_t AES_KEY[16] = {
        0x23, 0x1D, 0x39, 0xC1, 0xD7, 0xCC, 0x1A, 0xB1,
        0xAE, 0xE2, 0x24, 0xCD, 0x09, 0x6D, 0xB9, 0x32};

    // Short or complete name sent based on packet size
    BtHomeV2Device btHome("a", "b", false, AES_KEY, mac);

    btHome.setPowerState(Power_Sensor_Status_On);
    btHome.setPowerState(Power_Sensor_Status_On);

   
    TEST_ASSERT_EQUAL_STRING_MESSAGE("0201061016D2FC41CF244F9F01000000B4300FB4020962020861", getHexString(btHome).c_str(), "failed comparison");    
}

void test_encryptionCounter()
{
    const uint8_t mac[] = {0x72, 0x2E, 0x6C, 0x1B, 0x5A, 0xE0 };

    const uint8_t AES_KEY[16] = {
        0x23, 0x1D, 0x39, 0xC1, 0xD7, 0xCC, 0x1A, 0xB1,
        0xAE, 0xE2, 0x24, 0xCD, 0x09, 0x6D, 0xB9, 0x32};

    // Short or complete name sent based on packet size
    BtHomeV2Device btHome("a", "b", false, AES_KEY, mac);

    btHome.setPowerState(Power_Sensor_Status_On);
    btHome.setPowerState(Power_Sensor_Status_On);

   
    TEST_ASSERT_EQUAL_STRING_MESSAGE("0201061016D2FC41CF244F9F01000000B4300FB4020962020861", getHexString(btHome).c_str(), "failed comparison");    

    TEST_ASSERT_EQUAL_STRING_MESSAGE("0201061016D2FC419FEA0E8702000000D7AEF2F5020962020861", getHexString(btHome).c_str(), "incrementing counter failed");    
}


void test_encryptionSetCounter()
{
    const uint8_t mac[] = {0x72, 0x2E, 0x6C, 0x1B, 0x5A, 0xE0 };

    const uint8_t AES_KEY[16] = {
        0x23, 0x1D, 0x39, 0xC1, 0xD7, 0xCC, 0x1A, 0xB1,
        0xAE, 0xE2, 0x24, 0xCD, 0x09, 0x6D, 0xB9, 0x32};

    BtHomeV2Device btHome("a", "b", false, AES_KEY, mac, 12345); // setting counter

    btHome.setPowerState(Power_Sensor_Status_On);
    btHome.setPowerState(Power_Sensor_Status_On);

   
    TEST_ASSERT_EQUAL_STRING_MESSAGE("0201061016D2FC419677BC62393000006F61D992020962020861", getHexString(btHome).c_str(), "failed comparison");    

    TEST_ASSERT_EQUAL_STRING_MESSAGE("0201061016D2FC41150E05C93A300000AABABC84020962020861", getHexString(btHome).c_str(), "incrementing counter failed");    
}