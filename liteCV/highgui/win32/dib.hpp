#pragma once
#ifndef LCV_HIGHGUI_WIN32_DIB_HPP
#define LCV_HIGHGUI_WIN32_DIB_HPP

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // _WINDOWS_

#include "liteCV/core/lcvdef.hpp"
#include "liteCV/core/matrix.hpp"


namespace lcv
{
    class DIB
    {
    private:
        BITMAPINFO* pbmi;
        BYTE* pdata;

    private:
        DIB(const Matrix& mat)
        {
            int bpp, width, height;
            bpp = mat.elemSize() * 8;
            width = mat.cols;
            height = mat.rows;

            create(bpp, width, height);

            Matrix m;
            mat.copyTo(m);
            memcpy(pdata, m.data, width * height * ((int)bpp / 8));
        }
        
    public:
        DIB()
            : pbmi(NULL), pdata(NULL)
        {

        }

        ~DIB()
        {
            release();
        }

        DIB(const DIB& another)
        {
            deep_copy(another);
        }

        DIB(DIB&& another) noexcept
        {
            swallow_move(std::forward<DIB>(another));
        }

    public:
        DIB& operator=(const DIB& another)
        {
            deep_copy(another);
            return *this;
        }

        DIB& operator=(DIB&& another) noexcept
        {
            swallow_move(std::forward<DIB>(another));
            return *this;
        }

    private:
        bool is_same_buffer_size(const DIB& another) const
        {
            return Bpp() == another.Bpp() && Width() == another.Width() && Height() == another.Height();
        }

        void create(int bpp, int width, int height)
        {
            // Create BITMAPINFO
            pbmi = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + (256 * sizeof(RGBQUAD)));
            assert(pbmi != NULL);
            ZeroMemory(pbmi, sizeof(BITMAPINFO) + (256 * sizeof(RGBQUAD)));
            (*pbmi).bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            (*pbmi).bmiHeader.biWidth = width;
            (*pbmi).bmiHeader.biHeight = -height;
            (*pbmi).bmiHeader.biPlanes = 1;
            (*pbmi).bmiHeader.biBitCount = bpp;
            (*pbmi).bmiHeader.biCompression = BI_RGB;
            (*pbmi).bmiHeader.biSizeImage = width * height;

            // Make color palette if bpp is 8.
            if (bpp == 8)
            {
                for (int i = 0; i < 256; ++i)
                    (*pbmi).bmiColors[i] = RGBQUAD{ (BYTE)i, (BYTE)i, (BYTE)i, 0 };
            }

            // Create buffer
            pdata = (BYTE*)malloc(width * height * ((int)bpp / 8));
            assert(pdata != NULL);
        }

        void release()
        {
            // Release buffer
            if (pdata != NULL)
            {
                free(pdata);
                pdata = NULL;
            }

            // Release BITMAPINFO
            if (pbmi != NULL)
            {
                free(pbmi);
                pbmi = NULL;
            }
        }

        void deep_copy(const DIB& another)
        {
            // Do not need to allocate memory again if BITMAPINFO is same between this and another.
            if (!is_same_buffer_size(another))
            {
                // BITMAP is not same, allocate memory again.
                release();
                create(another.Bpp(), another.Width(), another.Height());
            }

            // Just copy pixels!
            memcpy(pdata, another.pdata, Width() * Height() * ((int)Bpp() / 8));
        }

        void swallow_move(DIB&& another)
        {
            release();
            pbmi = another.pbmi;
            pdata = another.pdata;
            another.pbmi = NULL;
            another.pdata = NULL;
        }

    public:
        static DIB FromMatrix(const Matrix& mat)
        {
            return DIB(mat);
        }

    public:
        bool IsEmpty() const
        {
            return pbmi == NULL || pdata == NULL;
        }

    public:
        int inline Bpp() const
        {
            return (*pbmi).bmiHeader.biBitCount;
        }

        int inline Width() const
        {
            return (*pbmi).bmiHeader.biWidth;
        }

        int inline Height() const
        {
            return std::abs((*pbmi).bmiHeader.biHeight);
        }

    public:
        void Draw(HWND hwnd, HDC hdc)
        {
            ::SetDIBitsToDevice(hdc, 0, 0, Width(), Height(), 0, 0, 0, Height(), pdata, pbmi, DIB_RGB_COLORS);
        }
    }; // class DIB
} // namespace lcv
#endif // LCV_HIGHGUI_WIN32_DIB_HPP
