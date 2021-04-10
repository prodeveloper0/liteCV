#pragma once
#ifndef LCV_CORE_SIZE_HPP
#define LCV_CORE_SIZE_HPP
#include "lcvdef.hpp"
#include "prototypes.hpp"

namespace lcv
{
    template<typename Type>
    class Size_
    {
    public:
        Type width, height;

    public:
        Size_() : width(0), height(0) {}
        Size_(Type _width, Type _height) : width(_width), height(_height) {}
        Size_(const Size_<Type>& size) : width(size.width), height(size.height) {}
        Size_(const Point_<Type>& pt) : width(pt.x), height(pt.y) {}

    public:
        Type area() const
        {
            return width * height;
        }

        bool empty() const
        {
            return width == 0 || height == 0;
        }

    public:
        template<typename AnotherType>
        operator Size_<AnotherType>()
        {
            return Size_<AnotherType>(width, height);
        }

    public:
        Size_<Type>& operator=(const Size_<Type>& size)
        {
            this->width = size.width;
            this->height = size.height;
            return *this;
        }
    }; // class Size_

    using Size2i = Size_<int32>;
    using Size2l = Size_<int64>;
    using Size2f = Size_<float32>;
    using Size2d = Size_<float64>;
    using Size = Size2i;
} // namespace lcv
#endif // LCV_CORE_SIZE_HPP
