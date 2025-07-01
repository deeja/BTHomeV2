#include <Arduino.h>
#include <bt_home.h>


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





}

void loop()
{
  // Do nothing
}
