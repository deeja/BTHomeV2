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


  uint8_t rawData[12] = {0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21};
  btHome.addRaw(rawData, sizeof(rawData));

  uint8_t buffer[MAX_PAYLOAD_SIZE];
  size_t size = btHome.getAdvertisementData(buffer);
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
