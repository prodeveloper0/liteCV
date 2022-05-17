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
    void resize(const Matrix& src, Matrix& dst, Size dsize, double fx = 0, double fy = 0, int interpolation = INTER_LINEAR)
    {
        // Only support 8-bits depth image
        assert(src.depth() == LCV_8U);

        // both dsize and fx|fy cannot be zero
        assert(dsize.area() == 0 && (fx == 0 || fy == 0));

        const int width = src.cols;
        const int height = src.rows;
        int scaled_width = dsize.width ? dsize.width : lcvRound(width * fx);
        int scaled_height = dsize.height ? dsize.height : lcvRound(height * fy);

        // scaled width and scaled height must not be zero
        assert(scaled_width * scaled_height != 0);

        Matrix output(scaled_width, scaled_height, src.type());

        // Loop height
        LCV_OMP_LOOP_FOR
        for (int y = 0; y < output.rows; ++y)
        {
            // Loop width
            for (int x = 0; x < output.cols; ++x)
            {
                const int forward_x = lcvRound(((float)x / scaled_width) * width);

                // Loop channel
                for (int ch = 0; ch < output.channels(); ++ch)
                {
                    InterpolationPolicy* ip = InterpolationPolicyStorage::get_policy(interpolation);
                    output.ptr<uchar>(y, x)[ch] = saturate_cast<uchar>(ip->interpolate(src, scaled_width, scaled_height, x, y, ch));
                } // Channel
            } // Width
        } // Height

        dst = output;
    } // resize
} // namespace lcv
#endif // LCV_IMGPROC_TRANSFORM_HPP
