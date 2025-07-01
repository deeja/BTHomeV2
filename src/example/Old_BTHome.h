#include "definitions.h"
#include <Arduino.h>
#include <bt_home/data_types.h>

class Old_BTHome {
  public:
    Old_BTHome(String device_name = "DIY-sensor",  bool trigger_based_device = false);
    std::string buildPacket();
    void resetMeasurement();
    bool addState(BtHomeType sensor,  uint8_t state, uint8_t steps = 0);
    bool addInteger(BtHomeType sensor, uint64_t value);
    bool addFloat(BtHomeType sensor, float value);
    bool addRaw(BtHomeType sensor, uint8_t *value, uint8_t size);

  private:
    bool pushBytes(uint64_t value2, BtHomeType sensor);
    uint8_t _sensorDataIdx = 0;    
    byte _sensorData[MEASUREMENT_MAX_LEN] = {0};
    String _device_name;
    bool _triggerdevice;
};
