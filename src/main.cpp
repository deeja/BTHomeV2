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

  BtHome btHome("Short", "Long Name", false);
  std::string result = btHome.getBytes();

  btHome.addCount_0_255(22);
  printBytes(btHome.getBytes());
  btHome.addCount_0_255(22);
  printBytes(btHome.getBytes());

}

void loop()
{
  // Do nothing
}
