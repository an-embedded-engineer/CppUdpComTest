#pragma once
#include <cstdint>
#include <stdexcept>

union Float32Int32Convert
{
    float f_value;
    int32_t i_value;
};

union Float32UInt32Convert
{
    float f_value;
    uint32_t i_value;
};

union Float64Int64Convert
{
    double f_value;
    int64_t i_value;
};

union Float64UInt64Convert
{
    double f_value;
    int64_t i_value;
};

template <typename TF, typename TI>
struct FloatIntConverter
{
    static TF ConvertToFloat(TI value)
    {
        throw std::out_of_range("Invalid Type");
    }

    static TI ConvertToInt(TF value)
    {
        throw std::out_of_range("Invalid Type");
    }
};

template <>
struct FloatIntConverter<float, int32_t>
{
    static float ConvertToFloat(int32_t value)
    {
        Float32Int32Convert convert;
        convert.i_value = value;
        return convert.f_value;
    }

    static int32_t ConvertToInt(float value)
    {
        Float32Int32Convert convert;
        convert.f_value = value;
        return convert.i_value;
    }
};

template <>
struct FloatIntConverter<float, uint32_t>
{
    static float ConvertToFloat(int32_t value)
    {
        Float32UInt32Convert convert;
        convert.i_value = value;
        return convert.f_value;
    }

    static uint32_t ConvertToInt(float value)
    {
        Float32UInt32Convert convert;
        convert.f_value = value;
        return convert.i_value;
    }
};

template <>
struct FloatIntConverter<double, int64_t>
{
    static double ConvertToFloat(int64_t value)
    {
        Float64Int64Convert convert;
        convert.i_value = value;
        return convert.f_value;
    }

    static int64_t ConvertToInt(double value)
    {
        Float64Int64Convert convert;
        convert.f_value = value;
        return convert.i_value;
    }
};

template <>
struct FloatIntConverter<double, uint64_t>
{
    static double ConvertToFloat(uint64_t value)
    {
        Float64UInt64Convert convert;
        convert.i_value = value;
        return convert.f_value;
    }

    static uint64_t ConvertToInt(double value)
    {
        Float64UInt64Convert convert;
        convert.f_value = value;
        return convert.i_value;
    }
};
