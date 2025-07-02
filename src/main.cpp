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
  NimBLEDevice::init("NimBLE");

  BtHomeV2Device btHome("DIY-sensor", "My DIY Sensor", false);
  btHome.addCount_0_255(22);

  uint8_t buffer[MAX_PAYLOAD_SIZE];
  size_t size = btHome.getAdvertisementData(buffer);

  NimBLEAdvertisementData pAdvData = BLEAdvertisementData();
  BLEAdvertisementData oScanResponseData = BLEAdvertisementData();
  std::vector<uint8_t> data(buffer, buffer + size);

  pAdvData.addData(data);

  auto pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->setAdvertisementData(pAdvData);
  oScanResponseData.setName("DIY-sensor");
  pAdvertising->setScanResponseData(oScanResponseData);
  pAdvertising->setConnectableMode(0);

  Serial.println("Starting advertising...");
  pAdvertising->start();
  delay(1000);
  pAdvertising->stop();

  delay(1000);

  Serial.println("Advertising data sent.");
}
