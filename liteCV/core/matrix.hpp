#pragma once
#ifndef LCV_CORE_MATRIX_HPP
#define LCV_CORE_MATRIX_HPP
#include <string>
#include <atomic>

#include "lcvdef.hpp"
#include "matrix.factory.hpp"
#include "rect.hpp"

namespace lcv
{
    class Matrix
    {
    public:
        int cols, rows;

        struct
        {
            int linestep;
            int elemstep;
        } step;

        struct
        {
            int depth;
            int nchans;
        } type;

        byte* data;
        byte* datastart;
        byte* dataend;
        byte* datalimit;

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
            cols = rows = 0;
            step.linestep = step.elemstep = 0;
            type.nchans = type.depth = 0;
            data = datastart = dataend = datalimit = NULL;
            refcount = nullptr;
        }

        void inline deep_copy(const Matrix& other)
        {
            create(other.cols, other.rows, other.type.nchans, other.type.depth);
            if (!other.isSubmatrix())
            {
                // Just copying data fully
                memcpy(data, other.data, step.linestep * rows);
            }
            else
            {
                // Copying data line by line
                for (int y = 0;y < other.rows;++y)
                {
                    memcpy(ptr(y), other.ptr(y), step.linestep);
                }
            }
        }

        void inline swallow_copy(const Matrix& other, bool must_be_released)
        {
            if (must_be_released)
                decref();
            this->cols = other.cols;
            this->rows = other.rows;
            this->step = other.step;
            this->type = other.type;
            this->refcount = other.refcount;
            this->data = other.data;
            this->datastart = other.datastart;
            this->dataend = other.dataend;
            this->datalimit = other.datalimit;
            incref();
        }

        void inline swallow_copy(const Matrix& other, const Rect& roi, bool must_be_released)
        {
            if (must_be_released)
                decref();
            this->cols = roi.width;
            this->rows = roi.height;
            this->step = other.step;
            this->type = other.type;
            this->refcount = other.refcount;
            this->data = (byte*)other.ptr(roi.y, roi.x);
            this->datastart = other.datastart;
            this->dataend = other.dataend;
            this->datalimit = other.datalimit;
            incref();
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
            // Copying with initializer
            swallow_copy(other, false);
        }

        Matrix(const Matrix& other, const Rect& roi)
        {
            // Copying with ROI with initializer
            swallow_copy(other, roi, false);
        }

        Matrix(int cols, int rows, const std::string channels = "8uc3")
        {
            // Creating with initializer
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

        Matrix operator()(const Rect& roi) const
        {
            // Copying with ROI by '()' operator
            Matrix sub_matrix;
            sub_matrix.swallow_copy(*this, roi, false);
            return sub_matrix;
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
            byte* datastart;
            byte* dataend;
            if ((datastart = (byte*)malloc(cols * rows * nchans * depth)) == NULL)
                throw std::bad_exception();
            dataend = datastart + (cols * rows * nchans * depth);

            // Update attributes
            decref();
            this->cols = cols;
            this->rows = rows;
            this->step.elemstep = (int)(depth * nchans) / 8;
            this->step.linestep = cols * step.elemstep;
            this->type.nchans = nchans;
            this->type.depth = depth;
            this->datastart = datastart;
            this->dataend = dataend;
            this->datalimit = datalimit;
            this->data = datastart;
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

        bool isSubmatrix() const
        {
            // Is it correct?
            // Why OpenCV uses that by using (cv::Mat::flags & SUBMATRIX_FLAG)?
            return data != datastart;
        }

    public:
        byte* ptr(int y=0)
        {
            return data + (step.linestep * y);
        }

        const byte* ptr(int y=0) const
        {
            return data + (step.linestep * y);
        }

        byte* ptr(int y, int x)
        {
            return data + (step.linestep * y) + (step.elemstep * x);
        }

        const byte* ptr(int y, int x) const
        {
            return data + (step.linestep * y) + (step.elemstep * x);
        }

        template<typename Element>
        Element* ptr(int y=0)
        {
            return (Element*)ptr(y);
        }

        template<typename Element>
        const Element* ptr(int y=0) const
        {
            return (Element*)ptr(y);
        }

        template<typename Element>
        Element* ptr(int y, int x)
        {
            return (Element*)ptr(y, x);
        }

        template<typename Element>
        const Element* ptr(int y, int x) const
        {
            return (Element*)ptr(y, x);
        }

    public:
        template<typename Element>
        Element& at(int y)
        {
            return *ptr<Element>(y);
        }

        template<typename Element>
        const Element& at(int y) const
        {
            return *ptr<Element>(y);
        }

        template<typename Element>
        Element& at(int y, int x)
        {
            return *ptr<Element>(y, x);
        }

        template<typename Element>
        const Element& at(int y, int x) const
        {
            return *ptr<Element>(y, x);
        }

        template<typename Element>
        Element& at(Point pt)
        {
            return at<Element>(pt.y, pt.x);
        }

        template<typename Element>
        const Element& at(Point pt) const
        {
            return at<Element>(pt.y, pt.x);
        }
    }; // class Matrix

    using Mat = Matrix;
} // namespace lcv
#endif // LCV_CORE_MATRIX_HPP
