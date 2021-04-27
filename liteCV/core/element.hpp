#pragma once
#ifndef LCV_CORE_ELEMENT_HPP
#define LCV_CORE_ELEMENT_HPP
#include "lcvdef.hpp"
#include "prototypes.hpp"

namespace lcv
{
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
} // namespace lcv
#endif // LCV_CORE_ELEMENT_HPP
