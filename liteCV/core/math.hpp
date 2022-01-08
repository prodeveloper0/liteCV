#pragma once
#ifndef LCV_CORE_MATH_HPP
#define LCV_CORE_MATH_HPP
#include "lcvdef.hpp"


namespace lcv
{
    template<typename T>
    T inline lcvModulo(T a, T b)
    {
        return (b + (a % b)) % b;
    } // lcvModulo

    template<typename Float>
    int32 inline lcvRound(Float v)
    {
        return (int)(v + ((Float)0.5));
    } // lcvRound
} // namespace lcv
#endif // LCV_CORE_MATH_HPP
