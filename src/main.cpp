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

  btHome.addHumidity_0_01(50.55f); // 41.4% humidity

  btHome.addHumidity_0_1(35.0f); // 41.4% humidity
  btHome.setBatteryState(BATTERY_STATE_LOW);

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
