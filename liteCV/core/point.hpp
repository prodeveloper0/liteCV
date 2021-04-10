#pragma once
#ifndef LCV_CORE_POINT_HPP
#define LCV_CORE_POINT_HPP
#include "lcvdef.hpp"
#include "prototypes.hpp"

namespace lcv
{
    template<typename Type>
    class Point_
    {
    public:
        Type x, y;

    public:
        Point_() : x(0), y(0) {}
        Point_(Type _x, Type _y) : x(_x), y(_y) {}
        Point_(const Point_<Type>& pt) : x(pt.x), y(pt.y) {}
        Point_(const Size_<Type> &size) : x(size.width), y(size.height) {}
        Point_(const Element_<Type, 2> &ele) : x(ele[0]), y(ele[1]) {}

    public:
        template<typename AnotherType>
        operator Point_<AnotherType>()
        {
            return Point_<AnotherType>(x, y);
        }

        template<typename AnotherType>
        operator Element_<AnotherType, 2>()
        {
            return Element_<AnotherType, 2>(x, y);
        }

    public:
        Point_<Type>& operator=(const Point_<Type>& pt)
        {
            this->x = pt.x;
            this->y = pt.y;
            return *this;
        }
    }; // class Point_

    template<typename Type>
    class Point3_
    {
    public:
        Type x, y, z;

    public:
        Point3_() : x(0), y(0), z(0) {}
        Point3_(Type _x, Type _y, Type _z) : x(_x), y(_y), z(_z) {}
        Point3_(const Point3_<Type>& pt) : x(pt.x), y(pt.y), z(pt.z) {}
        Point3_(const Point_<Type>& pt) : x(pt.x), y(pt.y), z(0) {}
        Point3_(const Element_<Type, 3>& ele) : x(ele[0]), y(ele[1]), z(ele[2]) {}

    public:
        template<typename AnotherType>
        operator Point3_<AnotherType>()
        {
            return Point3_<AnotherType>(x, y, z);
        }

        template<typename AnotherType>
        operator Element_<AnotherType, 3>()
        {
            return Element_<AnotherType, 3>(x, y, z);
        }

    public:
        Point3_<Type>& operator=(const Point3_<Type>& pt)
        {
            this->x = pt.x;
            this->y = pt.y;
            this->z = pt.z;
            return *this;
        }
    }; // class Point_

    using Point2i = Point_<int32>;
    using Point2l = Point_<int64>;
    using Point2f = Point_<float32>;
    using Point2d = Point_<float64>;
    using Point = Point2i;

    using Point3i = Point3_<int32>;
    using Point3f = Point3_<float32>;
    using Point3d = Point3_<float32>;
} // namespace lcv
#endif // LCV_CORE_POINT_HPP
