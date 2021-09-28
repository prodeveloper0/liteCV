#pragma once
#ifndef LCV_IMGPROC_COLOR_HPP
#define LCV_IMGPROC_COLOR_HPP
#include "liteCV/core/lcvdef.hpp"
#include "liteCV/core/matrix.hpp"


namespace lcv
{
    enum ColorConversionCodes
    {
        COLOR_RGB2BGR,
        COLOR_BGR2RGB,
        COLOR_BGR2BGRA,
        COLOR_BGR2GRAY,
        COLOR_RGBA2BGRA,
        COLOR_BGRA2RGBA,
        COLOR_BGRA2BGR,
        COLOR_BGRA2GRAY,
        COLOR_GRAY2BGR,
        COLOR_GRAY2BGRA
    }; // enum ColorConversionCodes


    /* ///////////////////////////////////////
    *  //    BGR - 3channels
    */ //
    void cvtColor_BGR2RGB(const Matrix& src, Matrix& dst)
    {
        assert(src.type() == LCV_8UC3);
        Matrix dst_image(src.cols, src.rows, LCV_8UC3);

        LCV_OMP_LOOP_FOR
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
    } // cvtColor_BGR2RGB

    void cvtColor_BGR2BGRA(const Matrix& src, Matrix& dst)
    {
        assert(src.type() == LCV_8UC3);
        Matrix dst_image(src.cols, src.rows, LCV_8UC4);

        LCV_OMP_LOOP_FOR
        for (int y = 0; y < src.rows; ++y)
        {
            const Vec3b* src_stride = (Vec3b*)src.ptr(y);
            Vec4b* dst_stride = (Vec4b*)dst_image.ptr(y);

            for (int x = 0; x < src.cols; ++x)
            {
                dst_stride[x][0] = src_stride[x][0];
                dst_stride[x][1] = src_stride[x][1];
                dst_stride[x][2] = src_stride[x][2];
                dst_stride[x][3] = 0;
            }
        }

        dst = dst_image;
    } // cvtColor_BGR2BGRA

    void cvtColor_BGR2GRAY(const Matrix& src, Matrix& dst)
    {
        assert(src.type() == LCV_8UC3);
        Matrix dst_image(src.cols, src.rows, LCV_8UC1);

        LCV_OMP_LOOP_FOR
        for (int y = 0; y < src.rows; ++y)
        {
            const Vec3b* src_stride = (Vec3b*)src.ptr(y);
            byte* dst_stride = dst_image.ptr(y);

            for (int x = 0; x < src.cols; ++x)
            {
                dst_stride[x] = (byte)(((int)src_stride[x][0] + src_stride[x][1] + src_stride[x][2]) / 3);
            }
        }

        dst = dst_image;
    } // cvtColor_BGR2GRAY


    /* ///////////////////////////////////////
    *  //    BGRA - 4channels
    */ //
    void cvtColor_BGRA2RGBA(const Matrix& src, Matrix& dst)
    {
        assert(src.type() == LCV_8UC4);
        Matrix dst_image(src.cols, src.rows, LCV_8UC4);

        LCV_OMP_LOOP_FOR
        for (int y = 0; y < src.rows; ++y)
        {
            const Vec4b* src_stride = (Vec4b*)src.ptr(y);
            Vec4b* dst_stride = (Vec4b*)dst_image.ptr(y);

            for (int x = 0; x < src.cols; ++x)
            {
                dst_stride[x][0] = src_stride[x][2];
                dst_stride[x][1] = src_stride[x][1];
                dst_stride[x][2] = src_stride[x][0];
                dst_stride[x][3] = src_stride[x][3];
            }
        }

        dst = dst_image;
    } // cvtColor_BGRA2RGBA

    void cvtColor_BGRA2BGR(const Matrix& src, Matrix& dst)
    {
        assert(src.type() == LCV_8UC4);
        Matrix dst_image(src.cols, src.rows, LCV_8UC3);

        LCV_OMP_LOOP_FOR
        for (int y = 0; y < src.rows; ++y)
        {
            const Vec4b* src_stride = (Vec4b*)src.ptr(y);
            Vec3b* dst_stride = (Vec3b*)dst_image.ptr(y);

            for (int x = 0; x < src.cols; ++x)
            {
                dst_stride[x][0] = src_stride[x][0];
                dst_stride[x][1] = src_stride[x][1];
                dst_stride[x][2] = src_stride[x][2];
            }
        }

        dst = dst_image;
    } // cvtColor_BGRA2BGR

    void cvtColor_BGRA2GRAY(const Matrix& src, Matrix& dst)
    {
        assert(src.type() == LCV_8UC4);
        Matrix dst_image(src.cols, src.rows, LCV_8UC1);

        LCV_OMP_LOOP_FOR
        for (int y = 0; y < src.rows; ++y)
        {
            const Vec4b* src_stride = (Vec4b*)src.ptr(y);
            byte* dst_stride = (byte*)dst_image.ptr(y);

            for (int x = 0; x < src.cols; ++x)
            {
                dst_stride[x] = (byte)(((int)src_stride[x][0] + src_stride[x][1] + src_stride[x][2]) / 3);
            }
        }

        dst = dst_image;
    } // cvtColor_BGRA2BGR


    /* ///////////////////////////////////////
    *  //    GRAY - 1channels
    */ //
    void cvtColor_GRAY2BGR(const Matrix& src, Matrix& dst)
    {
        assert(src.type() == LCV_8UC1);
        Matrix dst_image(src.cols, src.rows, LCV_8UC3);

        LCV_OMP_LOOP_FOR
        for (int y = 0; y < src.rows; ++y)
        {
            const byte* src_stride = src.ptr(y);
            Vec3b* dst_stride = (Vec3b*)dst_image.ptr(y);

            for (int x = 0; x < src.cols; ++x)
            {
                dst_stride[x][0] = src_stride[x];
                dst_stride[x][1] = src_stride[x];
                dst_stride[x][2] = src_stride[x];
            }
        }

        dst = dst_image;
    } // cvtColor_GRAY2BGR

    void cvtColor_GRAY2BGRA(const Matrix& src, Matrix& dst)
    {
        assert(src.type() == LCV_8UC1);
        Matrix dst_image(src.cols, src.rows, LCV_8UC4);

        LCV_OMP_LOOP_FOR
        for (int y = 0; y < src.rows; ++y)
        {
            const byte* src_stride = src.ptr(y);
            Vec4b* dst_stride = (Vec4b*)dst_image.ptr(y);

            for (int x = 0; x < src.cols; ++x)
            {
                dst_stride[x][0] = src_stride[x];
                dst_stride[x][1] = src_stride[x];
                dst_stride[x][2] = src_stride[x];
                dst_stride[x][3] = 0;
            }
        }

        dst = dst_image;
    } // cvtColor_GRAY2BGRA

    void cvtColor(const Matrix& src, Matrix& dst, int code)
    {
        switch (code)
        {
            // BGR -> ?
        case COLOR_RGB2BGR:
        case COLOR_BGR2RGB:
            cvtColor_BGR2RGB(src, dst);
            break;

        case COLOR_BGR2BGRA:
            cvtColor_BGR2BGRA(src, dst);
            break;

        case COLOR_BGR2GRAY:
            cvtColor_BGR2GRAY(src, dst);
            break;

            // BGRA -> ?
        case COLOR_RGBA2BGRA:
        case COLOR_BGRA2RGBA:
            cvtColor_BGRA2RGBA(src, dst);
            break;

        case COLOR_BGRA2BGR:
            cvtColor_BGRA2BGR(src, dst);
            break;

        case COLOR_BGRA2GRAY:
            cvtColor_BGRA2GRAY(src, dst);
            break;

            // GRAY -> ?
        case COLOR_GRAY2BGR:
            cvtColor_GRAY2BGR(src, dst);
            break;

        case COLOR_GRAY2BGRA:
            cvtColor_GRAY2BGRA(src, dst);
            break;
        }
    } // cvtColor
} // namespace lcv
#endif // LCV_IMGPROC_COLOR_HPP
