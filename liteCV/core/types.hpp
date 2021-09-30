#pragma once
#ifndef LCV_CORE_TYPES_HPP
#define LCV_CORE_TYPES_HPP
namespace lcv
{
    /* ///////////////////////////////////////
    *  //    Type declaration
    */ //
    template<typename Type, int N>
    class Element_;

    template<typename Type>
    class Point_;

    template<typename Type>
    class Point3_;

    template<typename Type>
    class Size_;

    template<typename Type>
    class Rect_;

    /* ///////////////////////////////////////
    *  //    Element_
    */ //
	template<typename Type, int N>
	class Element_
	{
	private:
		Type __Data[N];

	public:
		Element_()
		{
			memset(__Data, 0, sizeof(Type) * N);
		}

		Element_(Type a0, Type a1)
		{
			assert(N == 2);
			__Data[0] = a0;
			__Data[1] = a1;
		}

		Element_(Type a0, Type a1, Type a2)
		{
			assert(N == 3);
			__Data[0] = a0;
			__Data[1] = a1;
			__Data[2] = a2;
		}

		Element_(Type a0, Type a1, Type a2, Type a3)
		{
			assert(N == 4);
			__Data[0] = a0;
			__Data[1] = a1;
			__Data[2] = a2;
			__Data[3] = a3;
		}

		Element_(Type a0, Type a1, Type a2, Type a3, Type a4)
		{
			assert(N == 5);
			__Data[0] = a0;
			__Data[1] = a1;
			__Data[2] = a2;
			__Data[3] = a3;
			__Data[4] = a4;
		}

		Element_(Type a0, Type a1, Type a2, Type a3, Type a4, Type a5)
		{
			assert(N == 6);
			__Data[0] = a0;
			__Data[1] = a1;
			__Data[2] = a2;
			__Data[3] = a3;
			__Data[4] = a4;
			__Data[5] = a5;
		}

	public:
		Type& operator[](int i)
		{
			return __Data[i];
		}

		const Type& operator[](int i) const
		{
			return __Data[i];
		}
	}; // class Element_

    // Elements by type and channel
	using Element2b = Element_<uint8, 2>;
	using Element3b = Element_<uint8, 3>;
	using Element4b = Element_<uint8, 4>;
	using Element2s = Element_<int16, 2>;
	using Element3s = Element_<int16, 3>;
	using Element4s = Element_<int16, 4>;
	using Element2w = Element_<uint16, 2>;
	using Element3w = Element_<uint16, 3>;
	using Element4w = Element_<uint16, 4>;
	using Element2i = Element_<int32, 2>;
	using Element3i = Element_<int32, 3>;
	using Element4i = Element_<int32, 4>;
	using Element2f = Element_<float32, 2>;
	using Element3f = Element_<float32, 3>;
	using Element4f = Element_<float32, 4>;
	using Element2d = Element_<float64, 2>;
	using Element3d = Element_<float64, 3>;
	using Element4d = Element_<float64, 4>;

	// cv::Vec
	template<typename Type, int N>
	using Vec = Element_<Type, N>;

	using Vec2b = Vec<uint8, 2>;
	using Vec3b = Vec<uint8, 3>;
	using Vec4b = Vec<uint8, 4>;
	using Vec2s = Vec<int16, 2>;
	using Vec3s = Vec<int16, 3>;
	using Vec4s = Vec<int16, 4>;
	using Vec2w = Vec<uint16, 2>;
	using Vec3w = Vec<uint16, 3>;
	using Vec4w = Vec<uint16, 4>;
	using Vec2i = Vec<int32, 2>;
	using Vec3i = Vec<int32, 3>;
	using Vec4i = Vec<int32, 4>;
	using Vec2f = Vec<float32, 2>;
	using Vec3f = Vec<float32, 3>;
	using Vec4f = Vec<float32, 4>;
	using Vec2d = Vec<float64, 2>;
	using Vec3d = Vec<float64, 3>;
	using Vec4d = Vec<float64, 4>;

    /* ///////////////////////////////////////
    *  //    Point_, Point3_
    */ //
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
    }; // class Point3_

    using Point2i = Point_<int32>;
    using Point2l = Point_<int64>;
    using Point2f = Point_<float32>;
    using Point2d = Point_<float64>;
    using Point = Point2i;

    using Point3i = Point3_<int32>;
    using Point3f = Point3_<float32>;
    using Point3d = Point3_<float32>;

    /* ///////////////////////////////////////
    *  //    Size_
    */ //
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
    using Size2f = Size_<float32>;
    using Size2d = Size_<float64>;
    using Size = Size2i;

    /* ///////////////////////////////////////
    *  //    Rect_
    */ //
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
}; //namespace lcv
#endif // LCV_CORE_TYPES_HPP
