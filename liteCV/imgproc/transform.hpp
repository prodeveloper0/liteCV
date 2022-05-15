#pragma once
#ifndef LCV_IMGPROC_TRANSFORM_HPP
#include "liteCV/core/lcvdef.hpp"
#include "liteCV/core/lcvmath.hpp"
#include "liteCV/core/lcvtypes.hpp"
#include "liteCV/core/saturate.hpp"
#include "liteCV/core/matrix.hpp"

#include "interpolation.hpp"


namespace lcv
{
    void resize(const Matrix& src, Matrix& dst, Size dsize, double fx = 0, double fy = 0, int interpolation = INTER_NEAREST)
    {
        // Only support nearest neighborhood interpolation yet
        assert(interpolation == INTER_NEAREST);

        // Only support 8-bits depth image
        assert(src.depth() == LCV_8U);

        const int width = src.cols;
        const int height = src.rows;
        int scaled_width;
        int scaled_height;

        if (dsize.area()) 
        {
            // dsize is not zero
            scaled_width = dsize.width;
            scaled_height = dsize.height;
        }
        else
        {
            // dsize is zero, use scale factors(fx/fy)
            scaled_width = lcvRound(width * fx);
            scaled_height = lcvRound(height * fy);
        }

        // scaled width and scaled height must not be zero
        assert(scaled_width * scaled_height != 0);

        Matrix output(scaled_width, scaled_height, src.type());

        // Loop height
        LCV_OMP_LOOP_FOR
        for (int y = 0; y < output.rows; ++y)
        {
            const int forward_y = lcvRound(((float)y / scaled_height) * height);

            // Loop width
            for (int x = 0; x < output.cols; ++x)
            {
                const int forward_x = lcvRound(((float)x / scaled_width) * width);

                // Loop channel
                for (int ch = 0; ch < output.channels(); ++ch)
                {
                    output.ptr<uchar>(y, x)[ch] = src.ptr<uchar>(forward_y, forward_x)[ch];
                } // Channel
            } // Width
        } // Height

        dst = output;
    } // resize
} // namespace lcv
#endif // LCV_IMGPROC_TRANSFORM_HPP
