#ifndef BT_HOME_DATA_TYPES_H
#define BT_HOME_DATA_TYPES_H

#pragma once
#include <Arduino.h>
#include <vector>


union BtHomeValue
{
    uint8_t u8;
    int8_t s8;
    uint16_t u16;
    int16_t s16;
    uint32_t u32;
    int32_t s32;
    uint64_t u64;
    float f32;
    time_t time; // Assuming time_t is defined in your environment
};

typedef void (*SerializeFn)(std::vector<uint8_t> &out, BtHomeValue value);

struct BtHomeType
{
    uint8_t id;  // Type ID
    float scale; // Multiplier to apply before serializing
    SerializeFn serialize;
};

inline void serializeU8(std::vector<uint8_t> &out, BtHomeValue val)
{
    out.push_back(val.u8);
}

inline void serializeU16(std::vector<uint8_t> &out, BtHomeValue val)
{
    out.push_back(lowByte(val.u16));
    out.push_back(highByte(val.u16));
}

inline void serializeU32(std::vector<uint8_t> &out, BtHomeValue val)
{
    for (int i = 0; i < 4; ++i)
    {
        out.push_back((val.u32 >> (8 * i)) & 0xFF);
    }
}

inline void serializeU8(std::vector<uint8_t> &out, BtHomeValue val)
{
    out.push_back(static_cast<uint8_t>(val.u8));
}

inline void serializeU16(std::vector<uint8_t> &out, BtHomeValue val)
{
    uint16_t v = static_cast<uint16_t>(val.u16);
    out.push_back(lowByte(v));
    out.push_back(highByte(v));
}

inline void serializeU32(std::vector<uint8_t> &out, BtHomeValue val)
{
    uint32_t v = static_cast<uint32_t>(val.u32);
    for (int i = 0; i < 4; ++i)
    {
        out.push_back((v >> (8 * i)) & 0xFF);
    }
}

inline void serializeF32(std::vector<uint8_t> &out, BtHomeValue val)
{
    union
    {
        float f;
        uint8_t b[4];
    } u;
    u.f = val.f32;
    for (int i = 0; i < 4; ++i)
    {
        out.push_back(u.b[i]);
    }
}

// Helper for 24-bit unsigned serialization
inline void serializeU24(std::vector<uint8_t> &out, BtHomeValue val)
{
    uint32_t v = static_cast<uint32_t>(val.u32);
    out.push_back((v >> 0) & 0xFF);
    out.push_back((v >> 8) & 0xFF);
    out.push_back((v >> 16) & 0xFF);
}

// Helper for 8/16/24/32-bit signed serialization
inline void serializeS8(std::vector<uint8_t> &out, BtHomeValue val)
{
    out.push_back(static_cast<int8_t>(val.s8));
}
inline void serializeS16(std::vector<uint8_t> &out, BtHomeValue val)
{
    int16_t v = static_cast<int16_t>(val.s16);
    out.push_back(v & 0xFF);
    out.push_back((v >> 8) & 0xFF);
}
inline void serializeS32(std::vector<uint8_t> &out, BtHomeValue val)
{
    int32_t v = static_cast<int32_t>(val.s32);
    for (int i = 0; i < 4; ++i)
    {
        out.push_back((v >> (8 * i)) & 0xFF);
    }
}

inline void serializeTime(std::vector<uint8_t> &out, BtHomeValue val)
{
    uint64_t thing = val.time;
    BtHomeValue v;
    v.u64 = thing;
    serializeU48(out, v);
}

inline void serializeU48(std::vector<uint8_t> &out, BtHomeValue val)
{
    for (int i = 0; i < 6; ++i)
    {
        out.push_back((val.u64 >> (8 * i)) & 0xFF);
    }
}

const BtHomeType temperature_sint8 = {0x57, 1.0f, serializeS8};
const BtHomeType temperature_sint8_scale_0_35 = {0x58, 0.35f, serializeS8};
const BtHomeType temperature_sint16_scale_0_1 = {0x45, 0.1f, serializeS16};
const BtHomeType temperature_sint16_scale_0_01 = {0x02, 0.01f, serializeS16};

const BtHomeType count_uint8 = {0x09, 1.0f, serializeU8};
const BtHomeType count_uint16 = {0x3D, 1.0f, serializeU16};
const BtHomeType count_uint32 = {0x3E, 1.0f, serializeU32};
const BtHomeType count_sint8 = {0x59, 1.0f, serializeS8};
const BtHomeType count_sint16 = {0x5A, 1.0f, serializeS16};
const BtHomeType count_sint32 = {0x5B, 1.0f, serializeS32};

const BtHomeType voltage_0_001 = {0x0C, 0.001f, serializeU16};
const BtHomeType voltage_0_1 = {0x4A, 0.1f, serializeU16};

const BtHomeType battery_percentage = {0x01, 1.0f, serializeU8};

const BtHomeType distance_millimetre = {0x40, 1.0f, serializeU16};
const BtHomeType distance_metre = {0x41, 0.1f, serializeU16};




const BtHomeType acceleration = {0x51, 0.001f, serializeU16};
const BtHomeType channel = {0x60, 1.0f, serializeU8};
const BtHomeType co2 = {0x12, 1.0f, serializeU16};
const BtHomeType conductivity = {0x56, 1.0f, serializeU16};

const BtHomeType current_uint16 = {0x43, 0.001f, serializeU16};
const BtHomeType current_sint16 = {0x5D, 0.001f, serializeS16};
const BtHomeType dewpoint = {0x08, 0.01f, serializeS16};
const BtHomeType direction = {0x5E, 0.01f, serializeU16};
const BtHomeType duration_uint24 = {0x42, 0.001f, serializeU24};
const BtHomeType energy_uint32 = {0x4D, 0.001f, serializeU32};
const BtHomeType energy_uint24 = {0x0A, 0.001f, serializeU24};
const BtHomeType gas_uint24 = {0x4B, 0.001f, serializeU24};
const BtHomeType gas_uint32 = {0x4C, 0.001f, serializeU32};
const BtHomeType gyroscope = {0x52, 0.001f, serializeU16};
const BtHomeType humidity_uint16 = {0x03, 0.01f, serializeU16};
const BtHomeType humidity_uint8 = {0x2E, 1.0f, serializeU8};
const BtHomeType illuminance = {0x05, 0.01f, serializeU24};
const BtHomeType mass_kg = {0x06, 0.01f, serializeU16};
const BtHomeType mass_lb = {0x07, 0.01f, serializeU16};
const BtHomeType moisture_uint16 = {0x14, 0.01f, serializeU16};
const BtHomeType moisture_uint8 = {0x2F, 1.0f, serializeU8};
const BtHomeType pm2_5 = {0x0D, 1.0f, serializeU16};
const BtHomeType pm10 = {0x0E, 1.0f, serializeU16};
const BtHomeType power_uint24 = {0x0B, 0.01f, serializeU24};
const BtHomeType power_sint32 = {0x5C, 0.01f, serializeS32};
const BtHomeType precipitation = {0x5F, 0.1f, serializeU16};
const BtHomeType pressure = {0x04, 0.01f, serializeU24};
const BtHomeType rotation = {0x3F, 0.1f, serializeS16};
const BtHomeType speed = {0x44, 0.01f, serializeU16};
const BtHomeType timestamp = {0x50, 1.0f, serializeTime};
const BtHomeType tvoc = {0x13, 1.0f, serializeU16};

const BtHomeType volume_uint32 = {0x4E, 0.001f, serializeU32};
const BtHomeType volume_uint16_scale_0_1 = {0x47, 0.1f, serializeU16};
const BtHomeType volume_uint16_scale_1 = {0x48, 1.0f, serializeU16};
const BtHomeType volume_storage = {0x55, 0.001f, serializeU32};
const BtHomeType volume_flow_rate = {0x49, 0.001f, serializeU16};
const BtHomeType UV_index = {0x46, 0.1f, serializeU8};
const BtHomeType water_litre = {0x4F, 0.001f, serializeU32};
// text (0x53) requires custom serialization
// raw (0x54) and text (0x53) require custom serialization, not included here

#endif // BT_HOME_DATA_TYPES_H