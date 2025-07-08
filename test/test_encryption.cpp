#include "test_encryption.h"
#include "mbedtls/ccm.h"
#include <NimBLEDevice.h>

void test_encryptionPayload()
{

    NimBLEDevice::init("");

    const uint8_t *mac = NimBLEDevice::getAddress().getVal();

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

    NimBLEDevice::init("");

    const uint8_t *mac = NimBLEDevice::getAddress().getVal();

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

// void test_encryptionPayload()
// {

//     /*

// Flags:
// 0x60

// Counter:
// 00 00 00 01

// Nonce:
// F6 E5 D4 C3 B2 A1 D2 FC 60 00 00 00 01

// Plaintext:
// 04 02 08 CA (Type=0x04, Length=2, Value=0x08CA [=2250])
// */
//     const uint8_t AES_KEY[16] = {
//         0x00, 0x11, 0x22, 0x33,
//         0x44, 0x55, 0x66, 0x77,
//         0x88, 0x99, 0xAA, 0xBB,
//         0xCC, 0xDD, 0xEE, 0xFF};

//     const uint8_t MAC_ADDRESS[6] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6};

//     const uint8_t NONCE[12] = {0xF6, 0xE5, 0xD4, 0xC3, 0xB2, 0xA1, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00};

//     const uint8_t PLAINTEXT_PAYLOAD[7] = {0x04, 0x02, 0x08, 0xCA, 0x06, 0x01, 0x37};

//     const uint8_t CIPHERTEXT[7] = {0x4A, 0x5E, 0x3B, 0x72, 0xF3, 0xE4, 0xD0};

//     const uint8_t MIC[4] = {0x97, 0xD2, 0xEC, 0x5D};

//     const uint8_t FINAL_PACKET[17] = {
//         0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6,
//         0x4A, 0x5E, 0x3B, 0x72, 0xF3, 0xE4, 0xD0,
//         0x97, 0xD2, 0xEC, 0x5D};

//     BtHomeV2Device device("a", "a", false, AES_KEY, MAC_ADDRESS);

//     TEST_ASSERT_EQUAL_STRING_MESSAGE("0201060416D2FC41000000008C12E70F02 ---  ", getHexString(device).c_str(), "Encrypt fail");
// }
