#pragma once
#ifndef LCV_IMGPROC_FILTER_HPP
#include "liteCV/core/lcvdef.hpp"
#include "liteCV/core/matrix.hpp"
#include "liteCV/core/types.hpp"


namespace lcv
{
    void filter2D(const Matrix& src, Matrix& dst, uint32_t ddepth, const Matrix& kernel, Point anchor = Point(-1, -1), double delta = 0, int borderType = 4)
    {
        // Only support BORDER_DEFAULT
        assert(borderType == 4);

        // Only support 8-bits depth image
        assert(src.depth() == LCV_8U);
        assert(ddepth == LCV_8U);

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
                            const int ry = lcvModulo(y - (kernel.rows / 2) + ky + (anchor.y != -1 ? anchor.y : 0), output.rows);
                            const int rx = lcvModulo(x - (kernel.cols / 2) + kx + (anchor.x != -1 ? anchor.x : 0), output.cols);
                            sum += (krnl_scanline[kx] * (int)src.ptr<uint8_t>(ry, rx)[ch]);
                        }
                    } // Conolve

                    output.ptr<uint8_t>(y, x)[ch] = (uint8_t)(sum + delta);
                } // Channel
            } // Height 
        } // Width

        dst = output;
    } // filter2D
}; // namespace lcv
#endif // LCV_IMGPROC_FILTER_HPP
