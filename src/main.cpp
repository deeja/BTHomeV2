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
bool buttonOn = false;

void loop()
{
  NimBLEDevice::init("");
  BtHomeV2Device btHome("aaa", "aaa", false);

  // btHome.eventButton(Button_Event_Status_Hold_Press);
  if (buttonOn)
  {
    btHome.eventButton(Button_Event_Status_Press);
    btHome.eventButton(Button_Event_Status_Press);
    btHome.eventButton(Button_Event_Status_Press);
    btHome.eventDimmer(Dimmer_Event_Status_None, 0);

  }
  else
  {
     btHome.eventButton(Button_Event_Status_None);
     btHome.eventButton(Button_Event_Status_None);
     btHome.eventButton(Button_Event_Status_None);
    btHome.eventDimmer(Dimmer_Event_Status_RotateLeft, 10);
  }

  buttonOn = !buttonOn;

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

  delay(5000);

  Serial.println("Advertising data sent.");
}
