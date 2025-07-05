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
  NimBLEDevice::init("");
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

  // Short or complete name sent based on packet size
  BtHomeV2Device btHome("shosffr8", "My complete devnjjjema", false);

  btHome.setGasState(Gas_Sensor_Status_Detected);

  // The complete and short name will be sent if there is enough space in the packet; complete name has priority.
  // to make sure the name is sent out, send the packet without measurements
  size = btHome.getAdvertisementData(advertisementData);
  sendAdvertisement(advertisementData, size);
  btHome.clearMeasurementData();

  // if there is no more space in the packet, the add and set methods return false
  // as the byte sizes are fixed, you will likely only need this for debugging
  bool added = btHome.addHumidityPercent_Resolution_0_01(60.55f);
  if (!added)
  {
    Serial.println("No more space in packet. Send another packet with this measurement");
  }

  size = btHome.getAdvertisementData(advertisementData);
  sendAdvertisement(advertisementData, size);
  btHome.clearMeasurementData();

  btHome.setBatteryState(BATTERY_STATE_NORMAL);
  btHome.addBatteryPercentage(35);
  btHome.addVoltage_0_to_65_resolution_0_001(3.53);
  btHome.addCurrentAmps_0_65_Resolution_0_001(2.312);

  size = btHome.getAdvertisementData(advertisementData);
  sendAdvertisement(advertisementData, size);
  btHome.clearMeasurementData();

  // Many of the sensors can be stacked. The sensors in HA will be suffixed with _{index}
  btHome.addTemperature_neg127_to_127_Resolution_1(-22.1f);
  btHome.addTemperature_neg127_to_127_Resolution_1(102.6f);
  btHome.addTemperature_neg127_to_127_Resolution_1(2.0f);

  size = btHome.getAdvertisementData(advertisementData);
  sendAdvertisement(advertisementData, size);

  Serial.println("Delaying 5 seconds");
  delay(5000);
}
