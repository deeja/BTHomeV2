#include "Arduino.h"
#include "Old_BTHome.h"

void Old_BTHome::begin(String dname, bool trigger_based_device)
{
  setDeviceName(dname);
  this->m_triggerdevice = trigger_based_device;
  resetMeasurement();
}


void Old_BTHome::setDeviceName(String dname)
{
  if (!dname.isEmpty())
    this->dev_name = dname;
}

void Old_BTHome::resetMeasurement()
{
  this->m_sensorDataIdx = 0;  
}

bool Old_BTHome::addState(BtHomeType sensor, uint8_t state, uint8_t steps)
{
  uint8_t sensor_id = sensor.id;
  if ((this->m_sensorDataIdx + 2 + (steps > 0 ? 1 : 0)) <= (MEASUREMENT_MAX_LEN - (0)))
  {
    this->m_sensorData[this->m_sensorDataIdx] = static_cast<byte>(sensor_id & 0xff);
    this->m_sensorDataIdx++;
    this->m_sensorData[this->m_sensorDataIdx] = static_cast<byte>(state & 0xff);
    this->m_sensorDataIdx++;
    if (steps > 0)
    {
      this->m_sensorData[this->m_sensorDataIdx] = static_cast<byte>(steps & 0xff);
      this->m_sensorDataIdx++;
    }

  }
  else
  {
    addState(sensor, state, steps);
  }
}

bool Old_BTHome::addInteger(BtHomeType sensor, uint64_t value)
{
  uint8_t size = sensor.bytecount;
  uint16_t factor = sensor.scale;
  if ((this->m_sensorDataIdx + size + 1) <= (MEASUREMENT_MAX_LEN - (0)))
  {
    this->m_sensorData[this->m_sensorDataIdx] = static_cast<byte>(sensor.id & 0xff);
    this->m_sensorDataIdx++;
    for (uint8_t i = 0; i < size; i++)
    {
      this->m_sensorData[this->m_sensorDataIdx] = static_cast<byte>(((value / factor) >> (8 * i)) & 0xff);
      this->m_sensorDataIdx++;
    }

  }
  else
  {
    addInteger(sensor, value);
  }
}

bool Old_BTHome::addFloat(BtHomeType sensor, float value)
{
  uint8_t size = sensor.bytecount;
  uint16_t factor = sensor.scale;
  uint8_t sensor_id = sensor.id;
  
  if ((this->m_sensorDataIdx + size + 1) <= (MEASUREMENT_MAX_LEN - ( 0)))
  {
    uint64_t value2 = static_cast<uint64_t>(value / factor);
    this->m_sensorData[this->m_sensorDataIdx] = static_cast<byte>(sensor_id & 0xff);
    this->m_sensorDataIdx++;
    for (uint8_t i = 0; i < size; i++)
    {
      this->m_sensorData[this->m_sensorDataIdx] = static_cast<byte>((value2 >> (8 * i)) & 0xff);
      this->m_sensorDataIdx++;
    }

  }
  else
  {
    addFloat(sensor, value);
  }
}

// TEXT and RAW data
bool Old_BTHome::addBytes(BtHomeType sensor, uint8_t *value, uint8_t size)
{
  if ((this->m_sensorDataIdx + size + 1) <= (MEASUREMENT_MAX_LEN - ( 0)))
  {
    // Add sensor id
    this->m_sensorData[this->m_sensorDataIdx] = static_cast<byte>(sensor & 0xff);
    this->m_sensorDataIdx++;
    // Add data size, 1 byte
    this->m_sensorData[this->m_sensorDataIdx] = static_cast<byte>(size & 0xff);
    this->m_sensorDataIdx++;
    // Add data bytes
    for (uint8_t i = 0; i < size; i++)
    {
      this->m_sensorData[this->m_sensorDataIdx] = static_cast<byte>(value[i] & 0xff);
      this->m_sensorDataIdx++;
    }

  }
  else
  {
    addBytes(sensor, value, size);
  }
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

  if (this->m_triggerdevice)
    serviceData += NO_ENCRYPT_TRIGGER_BASE;
  else
    serviceData += NO_ENCRYPT;

  for (i = 0; i < this->m_sensorDataIdx; i++)
  {
    serviceData += this->m_sensorData[i]; // Add the sensor data to the Service Data
  }

  byte sd_length = serviceData.length(); // Generate the length of the Service Data
  payloadData += sd_length;              // Add the length of the Service Data
  payloadData += serviceData;            // Finalize the packet

  // Output payloadData as hex
  std::string hexStr;
  for (unsigned char c : payloadData) {
    char buf[3];
    snprintf(buf, sizeof(buf), "%02X", static_cast<unsigned char>(c));
    hexStr += buf;
  }
  return hexStr;
}

