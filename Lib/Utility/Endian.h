#pragma once
#include "FloatIntConverter.h"
#include "AppException.h"

#include <cstdint>
#include <type_traits>

enum class EndianType
{
    Big,
    Little,
};

namespace endian
{
    inline bool IsLittleEndian()
    {
        int data = 1;
        bool result = *reinterpret_cast<char*>(&data) == 1;
        return result;
    }

    inline bool IsBigEndian()
    {
        return !IsLittleEndian();
    }

    inline EndianType GetEnvironmentEndian()
    {
        if (IsLittleEndian() == true)
        {
            return EndianType::Little;
        }
        else
        {
            return EndianType::Big;
        }
    }

    namespace detail
    {
        template <typename T, size_t size>
        struct SwapBytes
        {
            inline T operator()(T value)
            {
                THROW_APP_EXCEPTION("Byte Size");
            }
        };

        template <typename T>
        struct SwapBytes<T, 1>
        {
            inline T operator()(T value)
            {
                return value;
            }
        };

        template <typename T>
        struct SwapBytes<T, 2>
        {
            inline T operator()(T value)
            {
                return ((((value) & 0xFF00) >> 8) |
                        (((value) & 0x00FF) << 8));
            }
        };

        template <typename T>
        struct SwapBytes<T, 4>
        {
            inline T operator()(T value)
            {
                return ((((value) & 0xFF000000) >> 24) |
                        (((value) & 0x00FF0000) >>  8) |
                        (((value) & 0x0000FF00) <<  8) |
                        (((value) & 0x000000FF) << 24));
            }
        };

        template <typename T>
        struct SwapBytes<T, 8>
        {
            inline T operator()(T value)
            {
                return ((((value) & 0xFF00000000000000ull) >> 56) |
                        (((value) & 0x00FF000000000000ull) >> 40) |
                        (((value) & 0x0000FF0000000000ull) >> 24) |
                        (((value) & 0x000000FF00000000ull) >>  8) |
                        (((value) & 0x00000000FF000000ull) <<  8) |
                        (((value) & 0x0000000000FF0000ull) << 24) |
                        (((value) & 0x000000000000FF00ull) << 40) |
                        (((value) & 0x00000000000000FFull) << 56));
            }
        };

        template <>
        struct SwapBytes<float, 4>
        {
            inline float operator()(float value)
            {
                uint32_t tmp_value = SwapBytes<uint32_t, sizeof(uint32_t)>()(FloatIntConverter<float, uint32_t>::ConvertToInt(value));
                return FloatIntConverter<float, uint32_t>::ConvertToFloat(tmp_value);
            }
        };

        template <>
        struct SwapBytes<double, 8>
        {
            inline double operator()(double value)
            {
                uint64_t tmp_value = SwapBytes<uint64_t, sizeof(uint64_t)>()(FloatIntConverter<double, uint64_t>::ConvertToInt(value));
                return FloatIntConverter<double, uint64_t>::ConvertToFloat(tmp_value);
            }
        };

        template<EndianType From, EndianType To, class T>
        struct Swap
        {
            inline T operator()(T value)
            {
                return SwapBytes<T, sizeof(T)>()(value);
            }
        };

        template <class T>
        struct Swap<EndianType::Little, EndianType::Little, T>
        {
            inline T operator()(T value)
            {
                return value;
            }
        };

        template <class T>
        struct Swap<EndianType::Big, EndianType::Big, T>
        {
            inline T operator()(T value)
            {
                return value;
            }
        };
    }

    template<EndianType From, EndianType To, class T>
    inline T ByteSwap(T value)
    {
        /* 入力データサイズチェック */
        static_assert((sizeof(T) == 1) || (sizeof(T) == 2) || (sizeof(T) == 4) || (sizeof(T) == 8), "(sizeof(T) == 1) || (sizeof(T) == 2) || (sizeof(T) == 4) || (sizeof(T) == 8)");
        /* 入力データ型チェック */
        static_assert((std::is_arithmetic<T>::value), "(std::is_arithmetic<T>::value)");
        /* エンディアン変換 */
        return detail::Swap<From, To, T>()(value);
    }

    template<class T>
    inline T ConvertEndian(T value)
    {
        /* 入力データサイズチェック */
        static_assert((sizeof(T) == 1) || (sizeof(T) == 2) || (sizeof(T) == 4) || (sizeof(T) == 8), "(sizeof(T) == 1) || (sizeof(T) == 2) || (sizeof(T) == 4) || (sizeof(T) == 8)");
        /* 入力データ型チェック */
        static_assert((std::is_arithmetic<T>::value), "(std::is_arithmetic<T>::value)");

        /* リトルエンディアン */
        if (GetEnvironmentEndian() == EndianType::Little)
        {
            /* エンディアン変換(リトル -> ビッグ) */
            return detail::Swap<EndianType::Little, EndianType::Big, T>()(value);
        }
        else
        {
            /* エンディアン変換(ビッグ -> リトル) */
            return detail::Swap<EndianType::Big, EndianType::Little, T>()(value);
        }
    }
}
