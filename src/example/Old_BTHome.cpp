#include "Arduino.h"
#include "Old_BTHome.h"

/// @brief Initialize the Old_BTHome object.
/// @param device_name
/// @param trigger_based_device
Old_BTHome::Old_BTHome(String device_name, bool trigger_based_device) : _device_name(device_name), _triggerdevice(trigger_based_device)
{
  _sensorDataIdx = 0; // Initialize the sensor data index
  memset(_sensorData, 0, sizeof(_sensorData));
}

/// @brief Clear the measurement data.
void Old_BTHome::resetMeasurement()
{
  _sensorDataIdx = 0;
}

/// @brief Add a state or step value to the sensor data packet.
/// @param sensor
/// @param state
/// @param steps
/// @return
bool Old_BTHome::addState(BtHomeType sensor, uint8_t state, uint8_t steps)
{
  if ((_sensorDataIdx + 2 + (steps > 0 ? 1 : 0)) <= (MEASUREMENT_MAX_LEN - (0)))
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

/// @brief Integer data
/// @param sensor
/// @param value
/// @return
bool Old_BTHome::addInteger(BtHomeType sensor, uint64_t value)
{
  uint8_t size = sensor.bytecount;
  if ((_sensorDataIdx + size + 1) > (MEASUREMENT_MAX_LEN))
  {
    return false;
  }

  uint64_t scaledValue = value / sensor.scale;;  
  return pushBytes(scaledValue, sensor);
}

/// @brief Float data
/// @param sensor
/// @param value
/// @return
bool Old_BTHome::addFloat(BtHomeType sensor, float value)
{
  if ((_sensorDataIdx + sensor.bytecount + 1) > (MEASUREMENT_MAX_LEN))
  {
    return false; // Not enough space for the data
  }

  float factor = sensor.scale;
  float scaledValue = value / factor;  
  return pushBytes(static_cast<uint64_t>(scaledValue), sensor);
}

bool Old_BTHome::pushBytes(uint64_t value2, BtHomeType sensor){
  _sensorData[_sensorDataIdx] = sensor.id;
  _sensorDataIdx++;
  for (uint8_t i = 0; i < sensor.bytecount; i++)
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
  if ((_sensorDataIdx + size + 1) > (MEASUREMENT_MAX_LEN))
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

  // Create the BLE Device
  std::string payloadData = "";
  std::string serviceData = "";
  uint8_t i;

  // head
  payloadData += FLAG1;
  payloadData += FLAG2;
  payloadData += FLAG3;

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
  payloadData += serviceDataLength;              // Add the length of the Service Data
  payloadData += serviceData;            // Finalize the packet

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
