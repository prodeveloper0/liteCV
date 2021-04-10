#pragma once
#ifndef LCV_CORE_RECT_HPP
#define LCV_CORE_RECT_HPP
#include "lcvdef.hpp"
#include "prototypes.hpp"

namespace lcv
{
    template<typename Type>
    class Rect_
    {
    public:
        Type x, y, width, height;

    public:
        Rect_() : x(0), y(0), width(0), height(0) {}
        Rect_(Type _x, Type _y, Type _width, Type _height)
        : x(_x), y(_y), width(_width), height(_height) {}
        Rect_(const Rect_& rect)
        : x(rect.x), y(rect.y), width(rect.width), height(rect.height) {}
        Rect_(const Point_<Type>& org, const Size_<Type>& size)
        : x(org.x), y(org.y), width(size.width), height(size.height) {}
        Rect_(const Point_<Type>& pt1, const Point_<Type>& pt2)
        : x(pt1.x), y(pt1.y), width(pt2.x - pt1.x), height(pt2.y - pt1.y) {}

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
        Point_<Type> tl() const
        {
            return Point_<Type>(x, y);
        }

        Point_<Type> br() const
        {
            return Point_<Type>(x + width, y + height);
        }

        Size_<Type> size() const
        {
            return Size_<Type>(width, height);
        }

    public:
        template<typename AnotherType>
        operator Rect_<AnotherType>()
        {
            return Rect_<AnotherType>(x, y, width, height);
        }

    public:
        Rect_<Type>& operator=(const Rect_<Type>& rect)
        {
            this->x = rect.x;
            this->y = rect.y;
            this->width = rect.width;
            this->height = rect.height;
            return *this;
        }
    }; // class Rect_

    using Rect2i = Rect_<int32>;
    using Rect2f = Rect_<float32>;
    using Rect2d = Rect_<float64>;
    using Rect = Rect2i;
} // namespace lcv
#endif // LCV_CORE_RECT_HPP
