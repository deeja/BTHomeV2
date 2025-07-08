#include <Arduino.h>
#include <BtHomeV2Device.h>
#include "NimBLEDevice.h"

void setup()
{
  Serial.begin(115200);
  delay(50);
  Serial.println("Starting BTHomeV2-Arduino Example...");
}
bool buttonOn = false;

void sendAdvertisement(uint8_t advertisementData[], size_t size)
{
  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
  NimBLEAdvertisementData pAdvData = BLEAdvertisementData();
  std::vector<uint8_t> data(advertisementData, advertisementData + size);

  pAdvData.addData(data);
  pAdvertising->setAdvertisementData(pAdvData);
  pAdvertising->setConnectableMode(0);
  Serial.println("Starting advertising...");
  pAdvertising->start();
  delay(1000);
  pAdvertising->stop();
  Serial.println("Stopping advertising...");
}

void loop()
{

  Serial.println("Starting example");

  uint8_t advertisementData[MAX_ADVERTISEMENT_SIZE];
  size_t size = 0;

  NimBLEDevice::init("");

  const uint8_t *mac = NimBLEDevice::getAddress().getVal();

  const uint8_t AES_KEY[16] = {
      0x23, 0x1D, 0x39, 0xC1, 0xD7, 0xCC, 0x1A, 0xB1,
      0xAE, 0xE2, 0x24, 0xCD, 0x09, 0x6D, 0xB9, 0x32};

  // Short or complete name sent based on packet size
  BtHomeV2Device btHome("a", "b", false, AES_KEY, mac);

  btHome.setPowerState(Power_Sensor_Status_On);
  btHome.setPowerState(Power_Sensor_Status_On);

  size = btHome.getAdvertisementData(advertisementData);
  sendAdvertisement(advertisementData, size);
  btHome.clearMeasurementData();

  Serial.println("Delaying 5 seconds");
  delay(5000);
}
