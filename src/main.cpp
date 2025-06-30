#include <Arduino.h>
#include "bt_home/bt_home.h"

void setup()
{
  Serial.begin(115200); // Initialize Serial Monitor for debugging
  delay(50);            // Wait for Serial Monitor to open
  ESP_LOGI("main", "Starting BT Home Example...");
  // put your setup code here, to run once:
  BtHome btHome1;

  ESP_LOGI("main", "Client name: %s", "aaaa");
  btHome1.addTemperature(25.0f, RANGE_127_RESOLUTION_1);
  btHome1.getBytes();

  ESP_LOGI("main", "Adding distance measurement: %f", 1.0f);
  btHome1.addDistanceMetres(1.0f);
  btHome1.getBytes();

  btHome1.addDistanceMillimetres(distance_millimetre.id, 1.0f);
  btHome1.getBytes();
}

void loop()
{
  // put your main code here, to run repeatedly:
}
