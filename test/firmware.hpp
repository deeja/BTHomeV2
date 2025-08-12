#ifndef FIRMWARE_HPP
#define FIRMWARE_HPP
#include "base.h"

void test_addFirmware()
{
    Serial.println("Testing BtHome getBytes method...");
    BtHomeV2Device btHome("a", "a", false);
    TEST_ASSERT_EQUAL_STRING("0201060416D2FC40020961020861", getHexString(btHome).c_str());
    btHome.addFirmwareVersion3(37, 99, 2);
    TEST_ASSERT_EQUAL_STRING("0201060816D2FC40F2026325020961020861", getHexString(btHome).c_str());
    btHome.addFirmwareVersion4(38, 99, 0, 255);
    TEST_ASSERT_EQUAL_STRING("0201060D16D2FC40F1FF006326F2026325020961020861", getHexString(btHome).c_str());
}

#endif