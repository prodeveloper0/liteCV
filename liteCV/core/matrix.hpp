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

        void inline deep_copy(const Matrix& other)
        {
            create(other.cols, other.rows, other.type.nchans, other.type.depth);
            memmove(data, other.data, other.cols * other.rows * other.elemSize()); // CAUTION SELF REFERENCE
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
            // Copying initializer
            swallow_copy(other, false);
        }

        Matrix(Matrix&& other) noexcept
        {
            // Moving initializer
            swallow_move(std::forward<Matrix>(other), false);
        }

        Matrix(int cols, int rows, const std::string channels = "8uc3")
        {
            init();
            create(cols, rows, channels);
        }

    public:
        Matrix& operator=(const Matrix& other)
        {
            // Copying by '=' operator
            swallow_copy(other, true);
            return *this;
        }

        Matrix& operator=(Matrix&& other) noexcept
        {
            // Moving by '=' operator
            swallow_move(std::forward<Matrix>(other), true);
            return *this;
        }

        template<typename Element>
        Matrix& operator=(const Element& value)
        {
            // setTo by '=' operator
            setTo(value);
            return *this;
        }

    public:
        static Matrix zeros(int cols, int rows, const std::string channels = "8uc3")
        {
            Matrix m(cols, rows, channels);
            memset(m.ptr(), 0, cols * rows * ((int)m.type.depth / 8) * m.type.nchans);
            return m;
        }

    private:
        // ONLY USES FOR CREATE MATRIX IN THE CLASS
        void create(int cols, int rows, int nchans, int depth)
        {
            // Allocate memory first
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
        void create(int cols, int rows, const std::string channels = "8uc3")
        {
            // Parse channels string
            int nchans, depth;
            parse_channels(channels, nchans, depth);

            // Create matrix by premitives
            create(cols, rows, nchans, depth);
        }

        template<typename Element>
        void setTo(const Element& value)
        {
            // The size of value and matrix's element size must be same
            assert(sizeof(value) == elemSize());

            // Set all elements to single value
            for (int i = 0; i < cols * rows; ++i)
                ptr<Element>()[i] = value;
        }

        void copyTo(Matrix& matrix) const
        {
            Matrix m;
            m.deep_copy(*this);
            matrix = m;
        }

        Matrix clone() const
        {
            Matrix m;
            copyTo(m);
            return m;
        }

    public:
        bool empty() const
        {
            return data == NULL;
        }

        size_t elemSize() const
        {
            return (int)(type.depth * type.nchans) / 8;
        }

        size_t elemSize1() const
        {
            return (int)type.depth / 8;
        }

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
            return &this->data[y * cols * elemSize()];
        }

        const byte* cptr(int y) const
        {
            return &this->data[y * cols * elemSize()];
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
