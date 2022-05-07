#pragma once
#ifndef LCV_IMGPROC_FILTER_HPP
#include "liteCV/core/lcvdef.hpp"
#include "liteCV/core/lcvmath.hpp"
#include "liteCV/core/lcvtypes.hpp"
#include "liteCV/core/saturate.hpp"
#include "liteCV/core/matrix.hpp"

#include "border.hpp"


namespace lcv
{
    void filter2D(const Matrix& src, Matrix& dst, int ddepth, const Matrix& kernel, Point anchor = Point(-1, -1), double delta = 0, int borderType = BORDER_DEFAULT)
    {
        // Only support 8-bits depth image
        // `-1` means same as source
        assert(src.depth() == LCV_8U && (ddepth == LCV_8U || ddepth == -1));

        // Kernel must be 32-bits float
        assert(kernel.depth() == LCV_32F);

        // Check sizes of matrix
        assert(kernel.cols % 2 != 0 && kernel.rows % 2 != 0);
        assert(src.cols > kernel.cols && src.rows > kernel.rows);

        Matrix output(src.cols, src.rows, src.type());

        // Loop width
        LCV_OMP_LOOP_FOR
        for (int y = 0; y < output.rows; ++y)
        {
            // Loop height
            for (int x = 0; x < output.cols; ++x)
            {
                // Loop channel
                for (int ch = 0; ch < output.channels(); ++ch)
                {
                    // Loop convolve
                    float sum = 0;
                    for (int ky = 0; ky < kernel.rows; ky++)
                    {
                        const float* krnl_scanline = kernel.ptr<float>(ky);
                        for (int kx = 0; kx < kernel.cols; ++kx)
                        {
                            BorderPolicy* bp = BorderPolicyStorage::get_policy(borderType);

                            const int ry = bp->calculate(y - (kernel.rows / 2) + ky + (anchor.y != -1 ? anchor.y : 0), output.rows);
                            const int rx = bp->calculate(x - (kernel.cols / 2) + kx + (anchor.x != -1 ? anchor.x : 0), output.cols);

                            sum += (krnl_scanline[kx] * (int)src.ptr<uchar>(ry, rx)[ch]);
                        }
                    } // Conolve

                    output.ptr<uchar>(y, x)[ch] = saturate_cast<uchar>(sum + delta);
                } // Channel
            } // Height 
        } // Width

        dst = output;
    } // filter2D

    void boxFilter(const Matrix& src, Matrix& dst, int ddepth, Size ksize, Point anchor = Point(-1, -1), bool normalize = true, int borderType = BORDER_DEFAULT)
    {
        // Make box filter
        lcv::Mat kernel(ksize.width, ksize.height, 1, LCV_32F);
        for (int i = 0; i < kernel.cols * kernel.rows; ++i)
            kernel.ptr<float>()[i] = normalize ? (1.f / (ksize.width * ksize.height)) : 1.f;

        // Convolve filter
        filter2D(src, dst, ddepth, kernel, anchor, 0., borderType);
    } // boxFilter

    void blur(const Matrix& src, Matrix& dst, Size size, Point anchor = Point(-1, -1), int borderType = BORDER_DEFAULT)
    {
        boxFilter(src, dst, src.depth(), size, anchor, true, borderType);
    } // blur
}; // namespace lcv
#endif // LCV_IMGPROC_FILTER_HPP
