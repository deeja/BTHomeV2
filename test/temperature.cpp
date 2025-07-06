
#include "temperature.h"

void test_addTemperature()
{
    Serial.println("Testing BtHome getBytes method...");
    BtHomeV2Device btHome("a", "a", false);
    TEST_ASSERT_EQUAL_STRING("0201060416D2FC40020961020861", getHexString(btHome).c_str());
    btHome.addTemperature_neg127_to_127_Resolution_1(-22.0f);
    TEST_ASSERT_EQUAL_STRING("0201060616D2FC4057EA020961020861", getHexString(btHome).c_str());
    btHome.addTemperature_neg44_to_44_Resolution_0_35(-7.7f);
    TEST_ASSERT_EQUAL_STRING("0201060816D2FC4057EA58EA020961020861", getHexString(btHome).c_str());
    btHome.addTemperature_neg327_to_327_Resolution_0_01(25.06f);
    TEST_ASSERT_EQUAL_STRING("0201060B16D2FC4057EA58EA02CA09020961020861", getHexString(btHome).c_str());
    btHome.addTemperature_neg3276_to_3276_Resolution_0_1(27.3f);
    TEST_ASSERT_EQUAL_STRING("0201060E16D2FC4057EA58EA02CA09451101020961020861", getHexString(btHome).c_str());
}