#pragma once
#ifndef LCV_CORE_SATURATE_HPP
#define LCV_CORE_SATURATE_HPP
#include <utility>

#include "lcvdef.hpp"
#include "lcvmath.hpp"


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
    T inline saturate_cast(uchar v)
    {
        return T(v);
    }

    template<typename T>
    T inline saturate_cast(schar v)
    {
        return T(v);
    }

    template<typename T>
    T inline saturate_cast(ushort v)
    {
        return T(v);
    }

    template<typename T>
    T inline saturate_cast(short v)
    {
        return T(v);
    }

    template<typename T>
    T inline saturate_cast(uint v)
    {
        return T(v);
    }

    template<typename T>
    T inline saturate_cast(int v)
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
    uchar inline saturate_cast<uchar>(schar v)
    {
        return clip<uchar, schar, int>(v, UINT8_MAX, 0);
    }

    template<>
    uchar inline saturate_cast<uchar>(ushort v)
    {
        return clip<uchar, ushort, uint>(v, UINT8_MAX);
    }

    template<>
    uchar inline saturate_cast<uchar>(short v)
    {
        return clip<uchar, short, int>(v, UINT8_MAX, 0);
    }

    template<>
    uchar inline saturate_cast<uchar>(uint v)
    {
        return clip<uchar, uint, uint>(v, UINT8_MAX);
    }

    template<>
    uchar inline saturate_cast<uchar>(int v)
    {
        return clip<uchar, int, int>(v, UINT8_MAX, 0);
    }

    template<>
    uchar inline saturate_cast<uchar>(float32 v)
    {
        return saturate_cast<uchar>(lcvRound(v));
    }

    template<>
    uchar inline saturate_cast<uchar>(float64 v)
    {
        return saturate_cast<uchar>(lcvRound(v));
    }

    template<>
    schar inline saturate_cast<schar>(uchar v)
    {
        return clip<schar, uchar, uint>(v, INT8_MAX);
    }

    template<>
    schar inline saturate_cast<schar>(ushort v)
    {
        return clip<schar, ushort, uint>(v, INT8_MAX);
    }

    template<>
    schar inline saturate_cast<schar>(short v)
    {
        return clip<schar, short, int>(v, INT8_MAX, INT8_MIN);
    }

    template<>
    schar inline saturate_cast<schar>(uint v)
    {
        return clip<schar, uint, uint>(v, INT8_MAX);
    }

    template<>
    schar inline saturate_cast<schar>(int v)
    {
        return clip<schar, int, int>(v, INT8_MAX, INT8_MIN);
    }

    template<>
    schar inline saturate_cast<schar>(float32 v)
    {
        return saturate_cast<schar>(lcvRound(v));
    }

    template<>
    schar inline saturate_cast<schar>(float64 v)
    {
        return saturate_cast<schar>(lcvRound(v));
    }

    template<>
    ushort inline saturate_cast<ushort>(schar v)
    {
        return clip<ushort, schar, int>(v, UINT16_MAX, 0);
    }

    template<>
    ushort inline saturate_cast<ushort>(short v)
    {
        return clip<ushort, short, int>(v, UINT16_MAX, 0);
    }

    template<>
    ushort inline saturate_cast<ushort>(uint v)
    {
        return clip<ushort, uint, uint>(v, UINT16_MAX);
    }

    template<>
    ushort inline saturate_cast<ushort>(int v)
    {
        return clip<ushort, int, int>(v, UINT16_MAX, 0);
    }

    template<>
    ushort inline saturate_cast<ushort>(float32 v)
    {
        return saturate_cast<ushort>(lcvRound(v));
    }

    template<>
    ushort inline saturate_cast<ushort>(float64 v)
    {
        return saturate_cast<ushort>(lcvRound(v));
    }

    template<>
    short inline saturate_cast<short>(ushort v)
    {
        return clip<short, ushort, uint>(v, INT16_MAX);
    }

    template<>
    short inline saturate_cast<short>(uint v)
    {
        return clip<short, uint, uint>(v, INT16_MAX);
    }

    template<>
    short inline saturate_cast<short>(int v)
    {
        return clip<short, int, int>(v, INT16_MAX, INT16_MIN);
    }

    template<>
    short inline saturate_cast<short>(float32 v)
    {
        return saturate_cast<short>(lcvRound(v));
    }

    template<>
    short inline saturate_cast<short>(float64 v)
    {
        return saturate_cast<short>(lcvRound(v));
    }

    template<>
    uint inline saturate_cast<uint>(schar v)
    {
        return (uint)clip<uint, schar, schar>(v, INT8_MAX, 0);
    }

    template<>
    uint inline saturate_cast<uint>(short v)
    {
        return (uint)clip<uint, short, short>(v, INT16_MAX, 0);
    }

    template<>
    uint inline saturate_cast<uint>(int v)
    {
        return (uint)clip<uint, int, int>(v, INT32_MAX, 0);
    }

    template<>
    uint inline saturate_cast<uint>(float32 v)
    {
        return (uint)lcvRound(v);
    }

    template<>
    uint inline saturate_cast<uint>(float64 v)
    {
        return (uint)lcvRound(v);
    }

    template<>
    int inline saturate_cast<int>(uint v)
    {
        return clip<int, uint, uint>(v, INT32_MAX);
    }

    template<>
    int inline saturate_cast<int>(float32 v)
    {
        return lcvRound(v);
    }

    template<>
    int inline saturate_cast<int>(float64 v)
    {
        return lcvRound(v);
    }
} // namespace lcv
#endif // LCV_CORE_SATURATE_HPP
