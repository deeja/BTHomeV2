#include "Arduino.h"
#include "Old_BTHome.h"

/// @brief
/// @param shortName - Short name of the device - sent when space is limited. Max 12 characters.
/// @param fullName - Full name of the device - sent when space is available.
/// @param isTriggerBased
Old_BTHome::Old_BTHome(const char *shortName, const char *fullName, bool isTriggerBased) : _shortName(shortName), _fullName(fullName), _triggerdevice(isTriggerBased)
{
  // 27 bytes left after flags and other headers
  _maximumMeasurementBytes = 27 - strlen(_shortName) - 2; // 2 bytes for the name id and size byte
}

/// @brief Clear the measurement data.
void Old_BTHome::resetMeasurement()
{
  _sensorDataIdx = 0;
}

/// @brief Check that there is enough space in the sensor data packet for the given size.
/// @details The sensor data packet has a maximum length defined by MEASUREMENT_MAX_LEN.
/// @param size
/// @return Returns true if there is enough space for the given size, false otherwise.
bool Old_BTHome::hasEnoughSpace(BtHomeType sensor)
{
  // minimum space is needed for the short name, but if there is spare room then we can use the full name
  uint8_t shortNameSize = strlen(_shortName) + 2; // 1 byte for the name id and one for the size byte
  return (_sensorDataIdx + sensor.byteCount + 1) <= _maximumMeasurementBytes;
}

/// @brief Add a state or step value to the sensor data packet.
/// @param sensor
/// @param state
/// @param steps
/// @return
bool Old_BTHome::addState(BtHomeType sensor, uint8_t state, uint8_t steps)
{
  if ((_sensorDataIdx + 2 + (steps > 0 ? 1 : 0)) <= _maximumMeasurementBytes)
  {
    _sensorData[_sensorDataIdx] = sensor.id;
    _sensorDataIdx++;
    _sensorData[_sensorDataIdx] = static_cast<byte>(state & 0xff);
    _sensorDataIdx++;
    if (steps > 0)
    {
      _sensorData[_sensorDataIdx] = static_cast<byte>(steps & 0xff);
      _sensorDataIdx++;
    }
  }
  else
  {
    return addState(sensor, state, steps);
  }
  return true;
}

bool Old_BTHome::addUnsignedInteger(BtHomeType sensor, uint64_t value)
{
  return addInteger(sensor, value);
}

bool Old_BTHome::addSignedInteger(BtHomeType sensor, int64_t value)
{
  return addInteger(sensor, value);
}

template <typename T>
bool Old_BTHome::addInteger(BtHomeType sensor, T value)
{
  if (!hasEnoughSpace(sensor))
  {
    return false;
  }
  auto scaledValue = static_cast<T>(static_cast<double>(value) / sensor.scale);
  return pushBytes(scaledValue, sensor);
}

/// @brief Float data
/// @param sensor
/// @param value
/// @return
bool Old_BTHome::addFloat(BtHomeType sensor, float value)
{
  if (!hasEnoughSpace(sensor))
  {
    return false;
  }

  float factor = sensor.scale;
  float scaledValue = value / factor;
  return pushBytes(static_cast<uint64_t>(scaledValue), sensor);
}

bool Old_BTHome::pushBytes(uint64_t value2, BtHomeType sensor)
{
  _sensorData[_sensorDataIdx] = sensor.id;
  _sensorDataIdx++;
  for (uint8_t i = 0; i < sensor.byteCount; i++)
  {
    _sensorData[_sensorDataIdx] = static_cast<byte>((value2 >> (8 * i)) & 0xff);
    _sensorDataIdx++;
  }
  return true;
}

/// @brief TEXT and RAW data
/// @param sensor
/// @param value
/// @param size
/// @return
bool Old_BTHome::addRaw(BtHomeType sensor, uint8_t *value, uint8_t size)
{
  if ((_sensorDataIdx + size + 1) > _maximumMeasurementBytes)
  // TODO: see if this can be moved to the hasEnoughSpace function
  {
    return false;
  }

  _sensorData[_sensorDataIdx] = sensor.id;
  _sensorDataIdx++;
  _sensorData[_sensorDataIdx] = static_cast<byte>(size & 0xff);
  _sensorDataIdx++;
  for (uint8_t i = 0; i < size; i++)
  {
    _sensorData[_sensorDataIdx] = static_cast<byte>(value[i] & 0xff);
    _sensorDataIdx++;
  }
  return true;
}

std::string Old_BTHome::buildPacket()
{  
  /**
   * 02 01 06                             ← Flags
   * 0B (length) -- 09 (name indicator) -- 44 49 59 2D 73 65 6E 73 6F 72  ← Complete Local Name: “DIY-sensor”
   * 0A (length) -- 16 (payload indicator) -- D2 FC 40 02 C4 09 03 BF 13    ← BTHome Service Data
   *
   * Or another example:
   * 02 01 06                                     │ Flags
   * 05 08 44 49 59 2D                            │ Length=5, AD type=0x08, "DIY-"
   * 0B 09 44 49 59 2D 73 65 6E 73 6F 72          │ Length=11, AD type=0x09, "DIY-sensor"
   * 0A 16 D2 FC 40 ...                           │ BTHome service data
   */

  // Create the BLE Device
  std::string payloadData = "";
  std::string serviceData = "";
  uint8_t i;

  // head
  payloadData += FLAG1;
  payloadData += FLAG2;
  payloadData += FLAG3;

  bool canFitLongName = (strlen(_fullName) - strlen(_shortName)) + _sensorDataIdx <= _maximumMeasurementBytes;

  if (canFitLongName)
  {

    payloadData += strlen(_fullName) + 1;
    payloadData += COMPLETE_NAME;
    payloadData += _fullName;
    
  }
  else
  {
    payloadData += strlen(_shortName) + 1;
    payloadData += SHORT_NAME;
    payloadData += _shortName;
  }

  serviceData += SERVICE_DATA; // DO NOT CHANGE -- Service Data - 16-bit UUID
  serviceData += UUID1;        // DO NOT CHANGE -- UUID
  serviceData += UUID2;        // DO NOT CHANGE -- UUID
                               // The encryption

  if (_triggerdevice)
    serviceData += NO_ENCRYPT_TRIGGER_BASE;
  else
    serviceData += NO_ENCRYPT;

  for (i = 0; i < _sensorDataIdx; i++)
  {
    serviceData += _sensorData[i]; // Add the sensor data to the Service Data
  }

  uint8_t serviceDataLength = serviceData.length(); // Generate the length of the Service Data
  Serial.println("Service Data Length: " + String(serviceDataLength));

  payloadData += serviceDataLength; // Add the length of the Service Data
  payloadData += serviceData;       // Finalize the packet

  // Output payloadData as hex
  std::string hexStr;
  for (unsigned char c : payloadData)
  {
    char buf[3];
    snprintf(buf, sizeof(buf), "%02X", static_cast<unsigned char>(c));
    hexStr += buf;
  }
  return hexStr;
}
