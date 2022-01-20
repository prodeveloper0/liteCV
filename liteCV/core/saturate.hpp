#pragma once
#ifndef LCV_CORE_SATURATE_HPP
#define LCV_CORE_SATURATE_HPP
#include <utility>

#include "lcvdef.hpp"
#include "math.hpp"


namespace lcv
{
    template<typename RType, typename IType, typename UType>
    RType clip(IType v, UType M)
    {
        return (RType)std::min((UType)v, (UType)M);
    }

    template<typename RType, typename IType, typename UType>
    RType clip(IType v, UType M, UType m)
    {
        return (RType)std::max(std::min((UType)v, (UType)M), (UType)m);
    }

    template<typename T>
    T inline saturate_cast(uint8 v)
    {
        return T(v);
    }

    template<typename T>
    T inline saturate_cast(int8 v)
    {
        return T(v);
    }

    template<typename T>
    T inline saturate_cast(uint16 v)
    {
        return T(v);
    }

    template<typename T>
    T inline saturate_cast(int16 v)
    {
        return T(v);
    }

    template<typename T>
    T inline saturate_cast(uint32 v)
    {
        return T(v);
    }

    template<typename T>
    T inline saturate_cast(int32 v)
    {
        return T(v);
    }

    template<typename T>
    T inline saturate_cast(float32 v)
    {
        return T(v);
    }

    template<typename T>
    T inline saturate_cast(float64 v)
    {
        return T(v);
    }

    template<>
    uint8 inline saturate_cast<uint8>(int8 v)
    {
        return clip<uint8, int8, int32>(v, UINT8_MAX, 0);
    }

    template<>
    uint8 inline saturate_cast<uint8>(uint16 v)
    {
        return clip<uint8, uint16, uint32>(v, UINT8_MAX);
    }

    template<>
    uint8 inline saturate_cast<uint8>(int16 v)
    {
        return clip<uint8, int16, int32>(v, UINT8_MAX, 0);
    }

    template<>
    uint8 inline saturate_cast<uint8>(uint32 v)
    {
        return clip<uint8, uint32, uint32>(v, UINT8_MAX);
    }

    template<>
    uint8 inline saturate_cast<uint8>(int32 v)
    {
        return clip<uint8, int32, int32>(v, UINT8_MAX, 0);
    }

    template<>
    uint8 inline saturate_cast<uint8>(float32 v)
    {
        return saturate_cast<uint8>(lcvRound(v));
    }

    template<>
    uint8 inline saturate_cast<uint8>(float64 v)
    {
        return saturate_cast<uint8>(lcvRound(v));
    }

    template<>
    int8 inline saturate_cast<int8>(uint8 v)
    {
        return clip<int8, uint8, uint32>(v, INT8_MAX);
    }

    template<>
    int8 inline saturate_cast<int8>(uint16 v)
    {
        return clip<int8, uint16, uint32>(v, INT8_MAX);
    }

    template<>
    int8 inline saturate_cast<int8>(int16 v)
    {
        return clip<int8, int16, int32>(v, INT8_MAX, INT8_MIN);
    }

    template<>
    int8 inline saturate_cast<int8>(uint32 v)
    {
        return clip<int8, uint32, uint32>(v, INT8_MAX);
    }

    template<>
    int8 inline saturate_cast<int8>(int32 v)
    {
        return clip<int8, int32, int32>(v, INT8_MAX, INT8_MIN);
    }

    template<>
    int8 inline saturate_cast<int8>(float32 v)
    {
        return saturate_cast<int8>(lcvRound(v));
    }

    template<>
    int8 inline saturate_cast<int8>(float64 v)
    {
        return saturate_cast<int8>(lcvRound(v));
    }

    template<>
    uint16 inline saturate_cast<uint16>(int8 v)
    {
        return clip<uint16, int8, int32>(v, UINT16_MAX, 0);
    }

    template<>
    uint16 inline saturate_cast<uint16>(int16 v)
    {
        return clip<uint16, int16, int32>(v, UINT16_MAX, 0);
    }

    template<>
    uint16 inline saturate_cast<uint16>(uint32 v)
    {
        return clip<uint16, uint32, uint32>(v, UINT16_MAX);
    }

    template<>
    uint16 inline saturate_cast<uint16>(int32 v)
    {
        return clip<uint16, int32, int32>(v, UINT16_MAX, 0);
    }

    template<>
    uint16 inline saturate_cast<uint16>(float32 v)
    {
        return saturate_cast<uint16>(lcvRound(v));
    }

    template<>
    uint16 inline saturate_cast<uint16>(float64 v)
    {
        return saturate_cast<uint16>(lcvRound(v));
    }

    template<>
    int16 inline saturate_cast<int16>(uint16 v)
    {
        return clip<int16, uint16, uint32>(v, INT16_MAX);
    }

    template<>
    int16 inline saturate_cast<int16>(uint32 v)
    {
        return clip<int16, uint32, uint32>(v, INT16_MAX);
    }

    template<>
    int16 inline saturate_cast<int16>(int32 v)
    {
        return clip<int16, int32, int32>(v, INT16_MAX, INT16_MIN);
    }

    template<>
    int16 inline saturate_cast<int16>(float32 v)
    {
        return saturate_cast<int16>(lcvRound(v));
    }

    template<>
    int16 inline saturate_cast<int16>(float64 v)
    {
        return saturate_cast<int16>(lcvRound(v));
    }

    template<>
    uint32 inline saturate_cast<uint32>(int8 v)
    {
        return (uint32)clip<uint32, int8, int8>(v, INT8_MAX, 0);
    }

    template<>
    uint32 inline saturate_cast<uint32>(int16 v)
    {
        return (uint32)clip<uint32, int16, int16>(v, INT16_MAX, 0);
    }

    template<>
    uint32 inline saturate_cast<uint32>(int32 v)
    {
        return (uint32)clip<uint32, int32, int32>(v, INT32_MAX, 0);
    }

    template<>
    uint32 inline saturate_cast<uint32>(float32 v)
    {
        return (uint32)lcvRound(v);
    }

    template<>
    uint32 inline saturate_cast<uint32>(float64 v)
    {
        return (uint32)lcvRound(v);
    }

    template<>
    int32 inline saturate_cast<int32>(uint32 v)
    {
        return clip<int32, uint32, uint32>(v, INT32_MAX);
    }

    template<>
    int32 inline saturate_cast<int32>(float32 v)
    {
        return lcvRound(v);
    }

    template<>
    int32 inline saturate_cast<int32>(float64 v)
    {
        return lcvRound(v);
    }
} // namespace lcv
#endif // LCV_CORE_SATURATE_HPP
