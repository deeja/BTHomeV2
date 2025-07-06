#include "base.h"


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