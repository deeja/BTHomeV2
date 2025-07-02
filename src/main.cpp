#include <Arduino.h>
#include <BtHomeV2Device.h>

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

  BtHomeV2Device btHome("DIY-sensor", "DIY-sensor", false);

  printBytes(btHome.getBytes());
  btHome.addCount_0_255(22);
  printBytes(btHome.getBytes());
  btHome.addCount_0_255(22);
  printBytes(btHome.getBytes());

}

void loop()
{
  // Do nothing
}
