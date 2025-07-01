#include <Arduino.h>
#include <bt_home.h>

// Overload for std::string (treat as raw bytes)
void printBytes(const std::string &bytes)
{
  Serial.println(String(bytes.c_str()));
}

void setup()
{
  Serial.begin(115200);
  delay(50); // Let serial settle

  Serial.println("Starting BT Home Example...");


  BtHome btHome("TestDevice", false);
  std::string result = btHome.getBytes();

  btHome.addTemperature(-22.0f, RANGE_127_RESOLUTION_1);
  printBytes(btHome.getBytes());

  btHome.addTemperature(-7.7f, RANGE_44_RESOLUTION_0_35);
  printBytes(btHome.getBytes());

  btHome.addTemperature(25.06f, RANGE_327_RESOLUTION_0_01);
  printBytes(btHome.getBytes());

  btHome.addTemperature(27.3f, RANGE_3276_RESOLUTION_0_1);
  printBytes(btHome.getBytes());



}

void loop()
{
  // Do nothing
}
