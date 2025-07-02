#include "bt_home.h"

void BtHome::clearMeasurementData()
{
    return _oldbthome.resetMeasurement();
}

/// @brief Builds an outgoing wrapper for the current measurement data.
/// @param payload
/// @return
std::string BtHome::getBytes()
{
    return _oldbthome.buildPacket(); // TODO: get the data
}

BtHome::BtHome(const char *deviceName, bool isTriggerDevice) : _oldbthome(deviceName, isTriggerDevice) // Initialize with default device name and trigger-based device flag
{
}

bool BtHome::addDistanceMetres(float metres)
{
    return _oldbthome.addFloat(distance_metre, metres);
}

bool BtHome::addDistanceMillimetres(float millimetres)
{
    return _oldbthome.addFloat(distance_millimetre, millimetres);
}

bool BtHome::addTemperature(float degreesCelsius, TemperatureRangeResolution rangeResolution)
{

    switch (rangeResolution)
    {
    case RANGE_44_RESOLUTION_0_35:
    {
        return _oldbthome.addFloat(temperature_int8_scale_0_35, degreesCelsius);
    }
    case RANGE_127_RESOLUTION_1:
    {
        return _oldbthome.addFloat(temperature_int8, degreesCelsius);
    }
    case RANGE_3276_RESOLUTION_0_1:
    {
        return _oldbthome.addFloat(temperature_int16_scale_0_1, degreesCelsius);
    }
    case RANGE_327_RESOLUTION_0_01:
    {
        return _oldbthome.addFloat(temperature_int16_scale_0_01, degreesCelsius);
    }
    }
    return false;
}

bool BtHome::addCount_0_4294967295(uint32_t count)
{
    return _oldbthome.addUnsignedInteger(count_uint32, count);
}
bool BtHome::addCount_0_255(uint8_t count)
{
    Serial.print("Adding count 0-255: ");
    Serial.println(count);
    return _oldbthome.addUnsignedInteger(count_uint8, count);
}
bool BtHome::addCount_0_65535(uint16_t count)
{
    return _oldbthome.addUnsignedInteger(count_uint16, count);
}
bool BtHome::addCount_neg128_127(int8_t count)
{
    return _oldbthome.addSignedInteger(count_int8, static_cast<uint64_t>(count));
}
bool BtHome::addCount_neg32768_32767(int16_t count)
{
    return _oldbthome.addSignedInteger(count_int16, static_cast<uint64_t>(count));
}
bool BtHome::addCount_neg2147483648_2147483647(int32_t count)
{
    return _oldbthome.addSignedInteger(count_int32, static_cast<uint64_t>(count));
}

bool BtHome::addVoltage(float voltage, VoltageRangeResolution rangeResolution)
{

    switch (rangeResolution)
    {
    case RANGE_65_RESOLUTION_0_001:
    {
        uint16_t value = static_cast<uint16_t>(voltage / voltage_0_001.scale);
        return addUInt16(voltage_0_1.id, value);
    }
    case RANGE_65535_RESOLUTION_0_1:
    {
        uint16_t value = static_cast<uint16_t>(voltage / voltage_0_1.scale);
        return addUInt16(voltage_0_1.id, value);
    }
    }
    return false;
}

bool BtHome::addBatteryPercentage(uint8_t batteryPercentage)
{
    return addUInt8(battery_percentage.id, batteryPercentage / battery_percentage.scale);
}
