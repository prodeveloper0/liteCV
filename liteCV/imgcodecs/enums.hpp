#pragma once
#ifndef LCV_IMGCODECS_ENUMS_HPP
#define LCV_IMGCODECS_ENUMS_HPP
#include "liteCV/core/lcvdef.hpp"

namespace lcv
{
    enum ImreadModes
    {
        IMREAD_UNCHANGED = -1,
        IMREAD_GRAYSCALE = 0,
        IMREAD_COLOR = 1,
        IMREAD_ANYDEPTH = 2,
        IMREAD_ANYCOLOR = 4
    };
} // namespace lcv
#endif // LCV_IMGCODECS_ENUMS_HPP
