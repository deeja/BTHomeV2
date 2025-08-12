

#ifndef DEVICE_TYPE_ID_HPP
#define DEVICE_TYPE_ID_HPP
#include "base.h"

void test_addDeviceTypeId()
{
    Serial.println("Testing BtHome getBytes method...");
    BtHomeV2Device btHome("a", "a", false);
    TEST_ASSERT_EQUAL_STRING("0201060416D2FC40020961020861", getHexString(btHome).c_str());
    btHome.addDeviceTypeId(3221);
    TEST_ASSERT_EQUAL_STRING("0201060716D2FC40F0950C020961020861", getHexString(btHome).c_str());
}

#endif