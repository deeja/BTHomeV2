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
const uint8_t btHomeServiceUuid[] = {0xD2, 0xFC};

void BtHome::clearMeasurementData()
{
    _measurementDataLength = 0;
    memset(_measurementData, 0, sizeof(_measurementData));
}

/// @brief Builds an outgoing wrapper for the current measurement data.
/// @param payload
/// @return
size_t BtHome::getAdvertisementPayload(uint8_t *advertisementData)
{
    uint8_t advertisementDataLength = 0;
    memcpy(advertisementData, btHomeFlags, sizeof(btHomeFlags));
    advertisementDataLength += sizeof(btHomeFlags);
  
    // copying the measurement data across to the outgoing advertisement data
    advertisementData[advertisementDataLength++] = _measurementDataLength; // Length of the payload from this point onwards
    memcpy(advertisementData, _measurementData, advertisementDataLength);
}

BtHome::BtHome()
{

    _measurementData[_measurementDataLength++] = 0x16; //  Service Data
    // Service UUID
    memcpy(_measurementData, btHomeServiceUuid, sizeof(btHomeServiceUuid));
    _measurementDataLength += sizeof(btHomeServiceUuid);
    _measurementData[_measurementDataLength++] = buildDeviceInfoByte(false, false, 2);    
}
/***
 * BTHome Device Information (0x40)
https://bthome.io/format/
The first byte after the UUID 0x40 is the BTHome device info byte, which has several bits indicating the capabilities of the device.

    bit 0: “Encryption flag”
        The Encryption flag is telling the receiver whether the device is sending non-encrypted data (bit 0 = 0) or encrypted data (bit 0 = 1).
    bit 1: “Reserved for future use”
    bit 2: “Trigger based device flag”
        The trigger based device flag is telling the receiver that it should expect that the device is sending BLE advertisements at a regular interval (bit 2 = 0) or at an irregular interval (bit 2 = 1), e.g. only when someone pushes a button. This can be useful information for a receiver, e.g. to prevent the device from going to unavailable.
    bit 3-4: “Reserved for future use”
    bit 5-7: “BTHome Version”
        This represents the BTHome verion. Currently only BTHome version 1 or 2 are allowed, where 2 is the latest version (bit 5-7 = 010).
 */
uint8_t buildDeviceInfoByte(bool encrypted, bool triggerBased, uint8_t version)
{
    version &= 0b00000111;                   // Mask to 3 bits
    return (encrypted ? 0b00000001 : 0) |    // bit 0
           (triggerBased ? 0b00000100 : 0) | // bit 2
           (version << 5);                   // bits 5–7
}

bool BtHome::addDistanceMetres(float metres)
{
    uint16_t value = static_cast<uint16_t>(metres / distance_metre.scale);
    return addUInt16(distance_metre.id, value);
}

bool BtHome::addDistanceMillimetres(float millimetres)
{
    uint16_t value = static_cast<uint16_t>(millimetres / distance_millimetre.scale);
    return addUInt16(distance_millimetre.id, value);
}

bool BtHome::addTemperature(float degreesCelsius, TemperatureRangeResolution rangeResolution)
{

    switch (rangeResolution)
    {
    case RANGE_127_RESOLUTION_1:
    {
        int8_t value = static_cast<int8_t>(degreesCelsius / temperature_sint8.scale);
        return addUInt8(temperature_sint8.id, value);
    }
    case RANGE_44_RESOLUTION_0_35:
    {
        int8_t value = static_cast<int8_t>(degreesCelsius / temperature_sint8_scale_0_35.scale);
        return addUInt8(temperature_sint8_scale_0_35.id, value);
    }
    case RANGE_3276_RESOLUTION_0_1:
    {
        int16_t value = static_cast<int16_t>(degreesCelsius / temperature_sint16_scale_0_1.scale);
        return addUInt16(temperature_sint16_scale_0_1.id, value);
    }
    case RANGE_327_RESOLUTION_0_01:
    {
        int16_t value = static_cast<int16_t>(degreesCelsius / temperature_sint16_scale_0_01.scale);
        return addUInt16(temperature_sint16_scale_0_01.id, value);
    }
    }
    return false;
}

bool BtHome::addCount(uint32_t count)
{
    return addUInt32(count_uint32.id, count / count_uint32.scale);
}
bool BtHome::addCount(uint8_t count)
{
    return addUInt8(count_uint8.id, count / count_uint8.scale);
}
bool BtHome::addCount(uint16_t count)
{
    return addUInt16(count_uint16.id, count / count_uint16.scale);
}
bool BtHome::addCount(int8_t count)
{
    return addUInt8(count_sint8.id, count / count_sint8.scale);
}
bool BtHome::addCount(int16_t count)
{
    return addUInt16(count_sint16.id, count / count_sint16.scale);
}
bool BtHome::addCount(int32_t count)
{
    return AddInt32(count_sint32.id, count / count_sint32.scale);
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

bool BtHome::addMeasurement(uint8_t id, const uint8_t *data, size_t len)
{
    if (_measurementDataLength + 1 + len > MAX_PAYLOAD_SIZE)
        return false; // prevent overflow
    _measurementData[_measurementDataLength++] = id;
    memcpy(_measurementData + _measurementDataLength, data, len);
    _measurementDataLength += len;
    return true;
}

bool BtHome::addFloat(uint8_t dataTypeId, float value)
{
    const uint8_t arrayLen = 2;
    uint16_t encoded = static_cast<uint16_t>(value);
    uint8_t data[arrayLen] = {
        static_cast<uint8_t>(encoded & 0xFF),
        static_cast<uint8_t>((encoded >> 8) & 0xFF)};
    return addMeasurement(dataTypeId, data, arrayLen);
}
bool BtHome::addUInt8(uint8_t dataTypeId, uint8_t value)
{
    const uint8_t arrayLen = 1;
    uint8_t data[arrayLen] = {value};
    return addMeasurement(dataTypeId, data, arrayLen);
}
bool BtHome::addUInt16(uint8_t dataTypeId, uint16_t value)
{
    const uint8_t arrayLen = 2;
    uint8_t data[arrayLen] = {
        static_cast<uint8_t>(value & 0xFF),
        static_cast<uint8_t>((value >> 8) & 0xFF)};
    return addMeasurement(dataTypeId, data, arrayLen);
}

bool BtHome::addUInt32(uint8_t dataTypeId, uint32_t value)
{
    const uint8_t arrayLen = 4;
    uint8_t data[arrayLen] = {
        static_cast<uint8_t>(value & 0xFF),
        static_cast<uint8_t>((value >> 8) & 0xFF),
        static_cast<uint8_t>((value >> 16) & 0xFF),
        static_cast<uint8_t>((value >> 24) & 0xFF)};
    return addMeasurement(dataTypeId, data, arrayLen);
}
bool BtHome::AddInt32(uint8_t dataTypeId, int32_t value)
{
    const uint8_t arrayLen = 4;
    uint8_t data[arrayLen] = {
        static_cast<uint8_t>(value & 0xFF),
        static_cast<uint8_t>((value >> 8) & 0xFF),
        static_cast<uint8_t>((value >> 16) & 0xFF),
        static_cast<uint8_t>((value >> 24) & 0xFF)};
    return addMeasurement(dataTypeId, data, arrayLen);
}
