#ifndef BT_HOME_H
#define BT_HOME_H

// https://bthome.io/format/

#include <Arduino.h>

/**
 * @file BTHome.h
 * @brief BTHome v2 sensor data setter methods with summaries.
 */

class BtHome
{
public:
    /**
     * @brief Set the temperature value in the packet.
     * @param degreesCelsius Temperature in degrees Celsius.
     */
    void setTemperature(float degreesCelsius);

    /**
     * @brief Set the humidity value in the packet.
     * @param humidityPercent Relative humidity in percent.
     */
    void setHumidity(float humidityPercent);

    /**
     * @brief Set the illuminance (light level) value in the packet.
     * @param illuminanceLux Ambient light in lux.
     */
    void setIlluminance(float illuminanceLux);

    /**
     * @brief Set the battery level value in the packet.
     * @param batteryPercentOrMillivolts Battery level as an unsigned 8-bit value (e.g., percentage or mV depending on implementation).
     */
    void setBattery(uint8_t batteryPercentOrMillivolts);

    /**
     * @brief Set the button state in the packet.
     * @param buttonState Button state as an unsigned 8-bit value (e.g., pressed/released).
     */
    void setButton(uint8_t buttonState);

    /**
     * @brief Set the soil moisture value in the packet.
     * @param moisturePercent Moisture level as a float (e.g., percentage).
     */
    void setMoisture(float moisturePercent);

    /**
     * @brief Set the conductivity value in the packet.
     * @param conductivityMicroSiemensCm Electrical conductivity in µS/cm.
     */
    void setConductivity(uint16_t conductivityMicroSiemensCm);

    /**
     * @brief Set the power (wattage) value in the packet.
     * @param powerWatts Power in watts.
     */
    void setPower(float powerWatts);

    /**
     * @brief Set the energy consumption value in the packet.
     * @param energyWattHours Energy in watt-hours.
     */
    void setEnergy(float energyWattHours);

    /**
     * @brief Set the voltage value in the packet.
     * @param voltageVolts Voltage in volts.
     */
    void setVoltage(float voltageVolts);

    /**
     * @brief Set the current value in the packet.
     * @param currentAmperes Current in amperes.
     */
    void setCurrent(float currentAmperes);

    /**
     * @brief Set the particulate matter (PM2.5) value in the packet.
     * @param pm2_5MicrogramsM3 PM2.5 concentration in µg/m³.
     */
    void setPM2_5(uint16_t pm2_5MicrogramsM3);

    /**
     * @brief Set the particulate matter (PM10) value in the packet.
     * @param pm10MicrogramsM3 PM10 concentration in µg/m³.
     */
    void setPM10(uint16_t pm10MicrogramsM3);

    /**
     * @brief Set the CO₂ concentration value in the packet.
     * @param co2PPM Carbon dioxide concentration in ppm.
     */
    void setCO2(uint16_t co2PPM);

    /**
     * @brief Set the total volatile organic compounds (TVOC) value in the packet.
     * @param tvocPPB TVOC concentration in ppb.
     */
    void setTVOC(uint16_t tvocPPB);

    /**
     * @brief Set the atmospheric pressure value in the packet.
     * @param pressureHpa Pressure in hPa.
     */
    void setPressure(float pressureHpa);

    /**
     * @brief Set the weight measurement value in the packet.
     * @param weightKg Weight in kilograms or grams depending on implementation.
     */
    void setWeight(float weightKg);

    /**
     * @brief Set the volume measurement value in the packet.
     * @param volumeLitres Volume in litres or cubic metres depending on implementation.
     */
    void setVolume(float volumeLitres);

    /**
     * @brief Set a generic count value in the packet.
     * @param count Arbitrary count (e.g., event count).
     */
    void setCount(uint32_t count);

    /**
     * @brief Set a duration value in the packet.
     * @param durationSeconds Duration in seconds or milliseconds depending on implementation.
     */
    void setDuration(uint32_t durationSeconds);

    /**
     * @brief Set the distance measurement value in the packet.
     * @param distanceMetres Distance in metres.
     */
    void setDistance(float distanceMetres);

    /**
     * @brief Set the speed measurement value in the packet.
     * @param speedMetresPerSecond Speed in metres per second or km/h.
     */
    void setSpeed(float speedMetresPerSecond);

    /**
     * @brief Set the ultraviolet (UV) index value in the packet.
     * @param uvIndex UV index (unitless).
     */
    void setUV(float uvIndex);

    /**
     * @brief Set a pulse count or heart rate value in the packet.
     * @param pulseBPM Pulse count or heart rate in beats per minute.
     */
    void setPulse(uint32_t pulseBPM);

    /**
     * @brief Set a generic floating-point value in the packet.
     * @param valueFloat Arbitrary float value.
     */
    void setGenericFloat(float valueFloat);

    /**
     * @brief Set a generic unsigned integer value in the packet.
     * @param valueUint Arbitrary unsigned integer value.
     */
    void setGenericUint(uint32_t valueUint);

    /**
     * @brief Set a generic signed integer value in the packet.
     * @param valueInt Arbitrary signed integer value.
     */
    void setGenericInt(int32_t valueInt);
};

#endif // BT_HOME_H