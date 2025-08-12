#ifndef PACKET_ID_HPP
#define PACKET_ID_HPP
#include "base.h"

void test_addPacketId()
{
    Serial.println("Testing BtHome getBytes method...");
    BtHomeV2Device btHome("a", "a", false);
    TEST_ASSERT_EQUAL_STRING("0201060416D2FC40020961020861", getHexString(btHome).c_str());
    btHome.addPacketId(243);    
    TEST_ASSERT_EQUAL_STRING("0201060616D2FC4000F3020961020861", getHexString(btHome).c_str());
}

#endif