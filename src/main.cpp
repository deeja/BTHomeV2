#include <Arduino.h>
#include "bt_home/bt_home.h"


// Overload for std::string (treat as raw bytes)
void printBytes(const std::string& bytes) {
  String hexString;
  for (size_t i = 0; i < bytes.size(); ++i) {
    uint8_t b = static_cast<uint8_t>(bytes[i]);
    if (b < 16) hexString += "0";
    hexString += String(b, HEX);
    if (i < bytes.size() - 1) hexString += " ";
  }
  Serial.println("Bytes (hex): " + hexString);
  Serial.println("Bytes (string): " + String(bytes.c_str()));
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

  btHome1.addTemperature(25.0f, RANGE_127_RESOLUTION_1);
  printBytes(btHome1.getBytes());

  Serial.print("Adding distance measurement: ");
  Serial.println(1.0f);

  btHome1.addDistanceMetres(1.0f);
  printBytes(btHome1.getBytes());

  btHome1.addDistanceMillimetres(1.0f);
  printBytes(btHome1.getBytes());
}

void loop()
{
  // Do nothing
}
