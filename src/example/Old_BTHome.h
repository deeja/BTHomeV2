#include "definitions.h"
#include <Arduino.h>
#include <bt_home/data_types.h>

class Old_BTHome {
  public:
    void begin(String dname = "DIY-sensor",  bool trigger_based_device = false);
    void setDeviceName(String dname = "");
    std::string buildPacket();
    void resetMeasurement();
    bool addState(BtHomeType sensor,  uint8_t state, uint8_t steps = 0);
    bool addInteger(BtHomeType sensor, uint64_t value);
    bool addFloat(BtHomeType sensor, float value);
    bool addBytes(BtHomeType sensor, uint8_t *value, uint8_t size);

  private:
    uint8_t m_sensorDataIdx;
    byte m_sensorData[MEASUREMENT_MAX_LEN] = {0};
    String dev_name;
    bool m_triggerdevice;
};
