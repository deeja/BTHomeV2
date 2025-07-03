#include <Arduino.h>
#include <BtHomeV2Device.h>
#include "NimBLEDevice.h"

// Overload for std::string (treat as raw bytes)
void printBytes(const std::string &bytes)
{
  Serial.println(String(bytes.c_str()));
}

void setup()
{
  Serial.begin(115200);
  delay(50);
  Serial.println("Starting BT Home Example...");
}

void loop()
{
  NimBLEDevice::init("");
  BtHomeV2Device btHome("aaa", "aaa", false);
  btHome.addTemperature(123.0f, RANGE_327_RESOLUTION_0_01);
  btHome.addTemperature(23.0f, RANGE_327_RESOLUTION_0_01);
  btHome.addBatteryPercentage(32);
  btHome.addVoltage(2.556f, RANGE_65_RESOLUTION_0_001);
  btHome.addVoltage(2566.7f, RANGE_65535_RESOLUTION_0_1);
  btHome.addVoltage(4866.9f, RANGE_65535_RESOLUTION_0_1);

  uint8_t buffer[MAX_PAYLOAD_SIZE];
  size_t size = btHome.getAdvertisementData(buffer);
  // Generated advertisement data:
  // 02 01 06 0e 09 4d 79 20 44 49 59 20 53 65 6e 73 6f 72 06 16 d2 fc 40 09 16
  // 02 01 06 (<- Initial Flags)
  // 0e (<- Length) 09 (<- Complete Name) 4d 79 20 44 49 59 20 53 65 6e 73 6f 72 (<- My DIY Sensor)
  // 06 (<- Length) 16 (<- Service Data ) d2 fc ( <- BTHome UUID )
  // 40 ( <- BTHomeV2 / Encryption / Trigger Device ) 09 (<- Count 0-255 )  16 (<-'25')

  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
  NimBLEAdvertisementData pAdvData = BLEAdvertisementData();
  std::vector<uint8_t> data(buffer, buffer + size);

  pAdvData.addData(data);
  pAdvertising->setAdvertisementData(pAdvData);
  pAdvertising->setConnectableMode(0);
  Serial.println("Starting advertising...");
  pAdvertising->start();
  delay(1000);
  pAdvertising->stop();

  delay(1000);

  Serial.println("Advertising data sent.");
}
