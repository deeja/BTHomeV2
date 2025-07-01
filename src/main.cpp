#include <Arduino.h>
#include "bt_home/bt_home.h"


// Overload for std::string (treat as raw bytes)
void printBytes(const std::string& bytes) {
  Serial.println(String(bytes.c_str()));
}

void setup()
{
  Serial.begin(115200);
  delay(50); // Let serial settle

  Serial.println("Starting BT Home Example...");
  
  // Create BtHome object
  BtHome btHome1("aaaa", false);
  printBytes(btHome1.getBytes());

  Serial.print("Client name: ");
  Serial.println("aaaa");

  btHome1.addTemperature(1.0f, RANGE_127_RESOLUTION_1);
  printBytes(btHome1.getBytes());
  btHome1.addTemperature(1.0f, RANGE_44_RESOLUTION_0_35);
  printBytes(btHome1.getBytes());
  btHome1.addTemperature(1.0f, RANGE_327_RESOLUTION_0_01);
  printBytes(btHome1.getBytes());
  btHome1.addTemperature(1.0f, RANGE_3276_RESOLUTION_0_1);
  printBytes(btHome1.getBytes());




}

void loop()
{
  // Do nothing
}
