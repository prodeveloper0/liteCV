#pragma once
#ifndef LCV_IMGPROC_BORDER_HPP
#define LCV_IMGPROC_BORDER_HPP
#include "liteCV/core/lcvdef.hpp"
#include <utility>


namespace lcv
{
    enum BorderTypes
    {
        BORDER_CONSTANT,
        BORDER_REPLICATE,
        BORDER_REFLECT,
        BORDER_REFLECT_101,
        BORDER_DEFAULT = BORDER_REFLECT_101,
    }; // enum BorderTypes

    class BorderPolicy
    {
    public:
        virtual bool is_constant() const = 0;
        virtual int calculate(int p, int max_boundary) = 0;
    }; // class BorderPolicy

    class ConstantBorderPolicy : public BorderPolicy
    {
    public:
        bool is_constant() const final
        {
            return true;
        }

        int calculate(int p, int max_boundary) final
        {
            return p;
        }
    }; // class ConstantBorderPolicy

    class ReplicateBorderPolicy : public BorderPolicy
    {
    public:
        bool is_constant() const final
        {
            return false;
        }

        int calculate(int p, int max_boundary) final
        {
            return std::max<int>(std::min<int>(p, max_boundary - 1), 0);
        }
    }; // class ReplicateBorderPolicy

    class ReflectBorderPolicy : public BorderPolicy
    {
    public:
        bool is_constant() const final
        {
            return false;
        }

        int calculate(int p, int max_boundary) final
        {
            if (p < 0)
                return std::min<int>(-p - 1, max_boundary - 1);
            else if (p >= max_boundary)
                return std::max<int>(max_boundary - (p - max_boundary) - 1, 0);
            return p;
        }
    }; // class ReflectBorderPolicy

    class Reflect101BorderPolicy : public BorderPolicy
    {
    public:
        bool is_constant() const final
        {
            return false;
        }

        int calculate(int p, int max_boundary) final
        {
            if (p < 0)
                return std::min<int>(-p, max_boundary - 1);
            else if (p >= max_boundary)
                return std::max<int>(max_boundary - (p - max_boundary) - 2, 0);
            return p;
        }
    }; // class Reflect101BorderPolicy

    class BorderPolicyStorage
    {
    private:
        BorderPolicyStorage() = delete;

    public:
        static BorderPolicy* get_policy(int type = BORDER_DEFAULT)
        {
            static ConstantBorderPolicy cbp;
            static ReplicateBorderPolicy repbp;
            static ReflectBorderPolicy refbp;
            static Reflect101BorderPolicy ref101bp;

            switch (type)
            {
            case BORDER_CONSTANT:
                return static_cast<BorderPolicy*>(&cbp);

            case BORDER_REPLICATE:
                return static_cast<BorderPolicy*>(&repbp);

            case BORDER_REFLECT:
                return static_cast<BorderPolicy*>(&refbp);

            case BORDER_REFLECT_101:
                return static_cast<BorderPolicy*>(&ref101bp);
            }

            return nullptr;
        }
    }; // class BorderPolicyStorage
} // namespace lcv
#endif // LCV_IMGPROC_BORDER_HPP
