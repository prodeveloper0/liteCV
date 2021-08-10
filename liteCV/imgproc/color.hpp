#pragma once
#ifndef LCV_IMGPROC_COLOR_HPP
#define LCV_IMGPROC_COLOR_HPP
#include "liteCV/core/lcvdef.hpp"
#include "liteCV/core/matrix.hpp"

namespace lcv
{
    enum ColorConversionCodes
    {
        COLOR_BGR2GRAY,
        COLOR_BGR2RGB,
        COLOR_RGB2GRAY,
        COLOR_RGB2BGR,
        COLOR_GRAY2BGR,
        COLOR_GRAY2RGB
    }; // enum ColorConversionCodes

    void cvtColor_RGB2GRAY(const Matrix& src, Matrix& dst)
    {
        assert(src.type.depth == 8 && src.type.nchans == 3);
        Matrix dst_image(src.cols, src.rows, "8uc1");
        
        //#pragma omp parallel for num_threads(4)
        for (int y = 0;y < src.rows;++y)
        {
            const Vec3b* src_stride = (Vec3b*)src.ptr(y);
            byte* dst_stride = dst_image.ptr(y);
            
            for (int x = 0;x < src.cols;++x)
            {
                dst_stride[x] = (byte)(((int)src_stride[x][0] + src_stride[x][1] + src_stride[x][2]) / 3);
            }
        }

        dst = dst_image;
    }


    void cvtColor_GRAY2RGB(const Matrix& src, Matrix& dst)
    {
        assert(src.type.depth == 8 && src.type.nchans == 1);
        Matrix dst_image(src.cols, src.rows, "8uc3");

        //#pragma omp parallel for num_threads(4)
        for (int y = 0;y < src.rows;++y)
        {
            const byte* src_stride = src.ptr(y);
            Vec3b* dst_stride = (Vec3b*)dst_image.ptr(y);

            for (int x = 0;x < src.cols;++x)
            {
                dst_stride[x][0] = src_stride[x];
                dst_stride[x][1] = src_stride[x];
                dst_stride[x][2] = src_stride[x];
            }
        }

        dst = dst_image;
    }


    void cvtColor_RGB2BGR(const Matrix& src, Matrix& dst)
    {
        assert(src.type.depth == 8 && src.type.nchans == 3);
        Matrix dst_image(src.cols, src.rows, "8uc3");

        //#pragma omp parallel for num_threads(4)
        for (int y = 0;y < src.rows;++y)
        {
            const Vec3b* src_stride = (Vec3b*)src.ptr(y);
            Vec3b* dst_stride = (Vec3b*)dst_image.ptr(y);

            for (int x = 0;x < src.cols;++x)
            {
                dst_stride[x][0] = src_stride[x][2];
                dst_stride[x][1] = src_stride[x][1];
                dst_stride[x][2] = src_stride[x][0];
            }
        }

        dst = dst_image;
    }


    void cvtColor(const Matrix& src, Matrix& dst, int code)
    {
        switch (code)
        {
        case COLOR_BGR2GRAY:
        case COLOR_RGB2GRAY:
            cvtColor_RGB2GRAY(src, dst);
            break;

        case COLOR_BGR2RGB:
        case COLOR_RGB2BGR:
            cvtColor_RGB2BGR(src, dst);
            break;

        case COLOR_GRAY2BGR:
        case COLOR_GRAY2RGB:
            cvtColor_GRAY2RGB(src, dst);
        }
    }
} // namespace lcv
#endif // LCV_IMGPROC_COLOR_HPP
