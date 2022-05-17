#pragma once
#ifndef LCV_IMGPROC_INTERPOLATION_HPP
#define LCV_IMGPROC_INTERPOLATION_HPP
#include "liteCV/core/lcvdef.hpp"
#include "liteCV/core/lcvmath.hpp"
#include "liteCV/core/matrix.hpp"


namespace lcv
{
    enum InterpolationFlags
    {
        INTER_NEAREST = 0,
        INTER_LINEAR = 1
    }; // enum InterpolationFlags

    class InterpolationPolicy
    {
    public:
        virtual double interpolate(const Matrix& src, int dwidth, int dheight, int dx, int dy, int ch) = 0;
    }; // class InterpolationPolicy

    class NearestInterpolationPolicy : public InterpolationPolicy
    {
    public:
        double interpolate(const Matrix& src, int dwidth, int dheight, int dx, int dy, int ch) final
        {
            const int sx = lcvRound(((float)dx / dwidth) * src.cols);
            const int sy = lcvRound(((float)dy / dheight) * src.rows);
            return (double)src.ptr<uchar>(sy, sx)[ch];
        }
    }; // class NearestInterpolationPolicy

    class LinearInterpolationPolicy : public InterpolationPolicy
    {
        double inline calc_ratio(double p)
        {
            return p - (int)p;
        }

    public:
        double interpolate(const Matrix& src, int dwidth, int dheight, int dx, int dy, int ch) final
        {
            // p is a point on src and can be mapped bacward from dst
            // a, b, c, d are neighbors of p
            // 
            // a -- b
            // |  p |
            // c -- d
            //
            // alpha is a distance of between a.x and p.x on src
            // beta is a distance of between a.y and p.y on src

            // a point on src mapped from dx, dy reversely
            const double sx = ((double)dx / dwidth) * src.cols;
            const double sy = ((double)dy / dheight) * src.rows;

            const double a = (double)src.ptr<uchar>(lcvFloor(sy), lcvFloor(sx))[ch];
            const double b = (double)src.ptr<uchar>(lcvFloor(sy), lcvCeil(sx))[ch];
            const double c = (double)src.ptr<uchar>(lcvCeil(sy), lcvFloor(sx))[ch];
            const double d = (double)src.ptr<uchar>(lcvCeil(sy), lcvCeil(sx))[ch];

            const double alpha = calc_ratio(sx);
            const double beta = calc_ratio(sy);

            const double e = (alpha * b) + ((1 - alpha) * a);
            const double f = (alpha * d) + ((1 - alpha) * c);

            return e + (beta * (f - e));
        }
    }; // class LinearInterpolationPolicy

    class InterpolationPolicyStorage
    {
    private:
        InterpolationPolicyStorage() = delete;

    public:
        static InterpolationPolicy* get_policy(int flag = INTER_NEAREST)
        {
            static NearestInterpolationPolicy nip;
            static LinearInterpolationPolicy lip;

            switch (flag)
            {
            case INTER_NEAREST:
                return static_cast<InterpolationPolicy*>(&nip);

            case INTER_LINEAR:
                return static_cast<InterpolationPolicy*>(&lip);
            }

            return nullptr;
        }
    }; // class InterpolationPolicyStorage
} // namespace lcv
#endif // LCV_IMGPROC_INTERPOLATION_HPP
