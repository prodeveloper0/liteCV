#pragma once
#ifndef LCV_CORE_LCVDEF_HPP
#define LCV_CORE_LCVDEF_HPP
#include <stdint.h>
#include <assert.h>


// OpenMP declaration macro
#ifdef _OPENMP
#ifdef _MSC_VER
#define LCV_OMP_LOOP_FOR __pragma(omp parallel for) 
#else
#define LCV_OMP_LOOP_FOR _Pragma("omp parallel for")
#endif
#else
#define LCV_OMP_LOOP_FOR
#endif


namespace lcv
{
    // Utility Classes
    class MovableOnly
    {
    public:
        MovableOnly() = default;

    private:
        MovableOnly(const MovableOnly&) = delete;
        MovableOnly& operator=(const MovableOnly&) = delete;
    }; // class MovableOnly

    class CopyableOnly
    {
    public:
        CopyableOnly() = default;

    private:
        CopyableOnly(CopyableOnly&&) = delete;
        CopyableOnly& operator=(CopyableOnly&&) = delete;
    }; // class CopyableOnly

    class Noncopyable : MovableOnly, CopyableOnly
    {
    public:
        Noncopyable() = default;
    }; // class Noncopyable

    // Premitive types
    using byte = uint8_t;
    using int8 = int8_t;
    using uint8 = uint8_t;
    using int16 = int16_t;
    using uint16 = uint16_t;
    using int32 = int32_t;
    using uint32 = uint32_t;
    using int64 = int64_t;
    using uint64 = uint64_t;
    using float32 = float;
    using float64 = double;
} // namespace lcv


// OpenCV's premitive types
using uchar = uint8_t;
using ushort = uint16_t;
#endif // LCV_CORE_LCVDEF_HPP
