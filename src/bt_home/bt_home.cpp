#include "bt_home.h"
#include "data_types.h"

/* Flags (0x01)

The flag value has several bits indicating the capabilities of the device. Although not required, it is still strongly advised to include this element in your advertisement,
 especially when using passive scanning on the receiving side. Bluez, which e.g. is used in the Home Assistant Bluetooth integration,
  won't be able to parse the data without this flags value. This part is always the same for BTHome (0x020106).
Flags data: 020106

    0x02 = length (2 bytes)
    0x01 = Flags
    0x06 = in bits, this is 00000110.
    bit 1 and bit 2 are 1, meaning:
    bit 1: “LE General Discoverable Mode”
    bit 2: “BR/EDR Not Supported”
*/

const uint8_t btHomeFlags[] = {0x02, 0x01, 0x06};



void BtHome::addTemperature(float degreesCelsius, TemperatureRangeResolution rangeResolution)
{

    switch (rangeResolution)
    {
    case RANGE_127_RESOLUTION_1:
    {
        int8_t value = static_cast<int8_t>(degreesCelsius / temperature_sint8.scale);
        // Serialize and add value as needed
        break;
    }
    case RANGE_44_RESOLUTION_0_35:
    {
        int8_t value = static_cast<int8_t>(degreesCelsius / temperature_sint8_scale_0_35.scale);
        // Serialize and add value as needed
        break;
    }
    case RANGE_3276_RESOLUTION_0_1:
    {
        int16_t value = static_cast<int16_t>(degreesCelsius / temperature_sint16_scale_0_1.scale);
        // Serialize and add value as needed
        break;
    }
    case RANGE_327_RESOLUTION_0_01:
    {
        int16_t value = static_cast<int16_t>(degreesCelsius / temperature_sint16_scale_0_01.scale);
        // Serialize and add value as needed
        break;
    }
    default:
        // Handle unknown resolution
        break;
    }
}

void BtHome::addCount(uint32_t count)
{
    count;
}
void BtHome::addCount(uint8_t count)
{
    count;
}
void BtHome::addCount(uint16_t count)
{
    count;
}
void BtHome::addCount(int8_t count)
{
    count;
}
void BtHome::addCount(int16_t count)
{
    count;
}
void BtHome::addCount(int32_t count)
{
    count;
}

void BtHome::addVoltage(float voltage, VoltageRangeResolution rangeResolution)
{

    switch (rangeResolution)
    {
    case RANGE_65_RESOLUTION_0_001:
    {
        uint16_t value = static_cast<uint16_t>(voltage / voltage_0_001.scale);
        // Serialize and add value as needed
        break;
    }
    case RANGE_65535_RESOLUTION_0_1:
    {
        uint16_t value = static_cast<uint16_t>(voltage / voltage_0_1.scale);
        // Serialize and add value as needed
        break;
    }
    default:
        // Handle unknown resolution
        break;
    }
}