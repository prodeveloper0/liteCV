#pragma once
#ifndef LCV_CORE_MATRIX_HPP
#define LCV_CORE_MATRIX_HPP
#include <string>
#include <atomic>

#include "lcvdef.hpp"
#include "matrix.factory.hpp"

namespace lcv
{
    class Matrix
    {
    public:
        int cols, rows;
        struct
        {
            uint16_t nchans;
            uint16_t depth;
        } type;
        byte* data;

    private:
        std::atomic<unsigned long>* refcount;

    private:
        void incref()
        {
            if (refcount == nullptr)
                refcount = new std::atomic<unsigned long>(0l);
            ++(*refcount);
        }

        void decref()
        {
            if (refcount != nullptr)
            {
                --(*refcount);
                if ((*refcount) == 0)
                {
                    if (data != NULL)
                        free(data);
                    delete refcount;
                }
            }
        }

    private:
        void init()
        {
            cols = rows = type.nchans = type.depth = 0;
            data = NULL;
            refcount = nullptr;
        }

        void inline swallow_copy(const Matrix& other, bool must_be_released)
        {
            if (must_be_released)
                decref();
            this->cols = other.cols;
            this->rows = other.rows;
            this->type = other.type;
            this->refcount = other.refcount;
            this->data = other.data;
            incref();
        }

        void inline swallow_move(Matrix&& other, bool must_be_released)
        {
            if (must_be_released)
                decref();
            this->cols = other.cols;
            this->rows = other.rows;
            this->type = other.type;
            this->refcount = other.refcount;
            this->data = other.data;
            other.init();
        }

    public:
        Matrix() noexcept
        {
            init();
        }

        virtual ~Matrix() noexcept
        {
            decref();
        }

        Matrix(const Matrix& other)
        {
            // Copy initializer
            swallow_copy(other, false);
        }

        Matrix(Matrix&& other) noexcept
        {
            // Move initializer
            swallow_move(std::move(other), false);
        }

        Matrix(int cols, int rows, const std::string channels = "8uc3")
        {
            init();
            create(cols, rows, channels);
        }

    public:
        Matrix& operator=(const Matrix& other)
        {
            // Copy operator
            swallow_copy(other, true);
            return *this;
        }

        Matrix& operator=(Matrix&& other) noexcept
        {
            // Move operator
            swallow_move(std::move(other), true);
            return *this;
        }

    public:
        void create(int cols, int rows, const std::string channels = "8uc3")
        {
            // Parse channels string
            int nchans, depth;
            parse_channels(channels, nchans, depth);

            // Allocate memory
            byte* data;
            if ((data = (byte*)malloc(cols * rows * nchans * depth)) == NULL)
                throw std::bad_exception();

            // Update attributes
            decref();
            this->cols = cols;
            this->rows = rows;
            this->type.nchans = (uint16_t)nchans;
            this->type.depth = (uint16_t)depth;
            this->data = data;
            incref();
        }

    public:
        byte* ptr()
        {
            return this->data;
        }

        const byte* cptr() const
        {
            return this->data;
        }

        byte* ptr(int y)
        {
            return &this->data[y * cols * ((int)type.depth / 8) * type.nchans];
        }

        const byte* cptr(int y) const
        {
            return &this->data[y * cols * ((int)type.depth / 8) * type.nchans];
        }

        template<typename Element>
        Element* ptr()
        {
            return (Element*)this->data;
        }

        template<typename Element>
        Element* cptr() const
        {
            return (Element*)this->data;
        }

        template<typename Element>
        Element* ptr(int y)
        {
            return &(((Element*)this->data)[y * cols]);
        }

        template<typename Element>
        Element* cptr(int y) const
        {
            return &(((Element*)this->data)[y * cols]);
        }

    public:
        template<typename Element>
        Element& at(int y)
        {
            ((Element*)this->data)[y * cols];
        }

        template<typename Element>
        Element& at(int y, int x)
        {
            return ((Element*)this->data)[y * cols + x];
        }
    }; // class Matrix

    using Mat = Matrix;
} // namespace lcv
#endif // LCV_CORE_MATRIX_HPP
