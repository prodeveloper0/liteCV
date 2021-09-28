#pragma once
#ifndef LCV_IMGCODECS_HPP
#define LCV_IMGCODECS_HPP
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>

#include "liteCV/core/lcvdef.hpp"
#include "liteCV/core/matrix.hpp"
#include "liteCV/imgproc/color.hpp"
#include "flags.hpp"

// Disable some stb's image codecs except JPG, PNG and BMP.
#define STBI_NO_GIF
#define STBI_NO_PSD
#define STBI_NO_PIC
#define STBI_NO_PNM
#define STBI_NO_HDR
#define STBI_NO_TGA

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"


namespace lcv
{
    Matrix imread(const std::string& filename, int flag = IMREAD_UNCHANGED)
    {
        Matrix img;
        stbi_uc* data;
        int width, height, channels;

        // Set `req_comp` by flag
        int req_comp;
        if (flag == IMREAD_COLOR)
        {
            req_comp = 3;
        }
        else if (flag == IMREAD_GRAYSCALE)
        {
            req_comp = 1;
        }
        else if (flag == IMREAD_UNCHANGED)
        {
            req_comp = 0;
        }
        else
        {
            assert(0 && "Unknown flag");
        }

        // Load image from file using stb_image
        data = stbi_load(filename.c_str(), &width, &height, &channels, req_comp);
        if (data == NULL)
            goto ret;

        // Copy from buffer
        img.create(width, height, req_comp == 0 ? channels : req_comp, LCV_8U);
        memcpy(img.ptr(), data, (size_t)height * img.step_info.linestep);
        stbi_image_free(data);

        // Change pixel order
        if (img.channels() == 3)
            cvtColor(img, img, COLOR_RGB2BGR);
        else if (img.channels() == 4)
            cvtColor(img, img, COLOR_RGBA2BGRA);

        ret:
        return img;
    } // imread

    bool imwrite(const std::string& filename, const Matrix& img, const std::vector<int>& params = std::vector<int>())
    {
        // Only write grayscale or color (3ch/4ch) image
        assert(img.channels() == 1 || img.channels() == 3 || img.channels() == 4);

        Matrix _img;

        // `std::filesystem` only can use in C++17 :(
        const std::filesystem::path path = filename;

        // Get image file extension from file name to encode image
        std::string extension = path.extension().string();
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

        // Change pixel order
        if (img.channels() == 3)
            cvtColor(img, _img, COLOR_RGB2BGR);
        else if (img.channels() == 4)
            cvtColor(img, _img, COLOR_BGRA2RGBA);

        // Write image to file to use proper image encoder by the extension
        // `params` is not used yet :(
        if (extension == ".bmp")
        {
            return stbi_write_bmp(filename.c_str(), _img.cols, _img.rows, _img.channels(), _img.ptr()) != 0;
        }
        else if (extension == ".jpg" || extension == ".jpeg")
        {
            return stbi_write_jpg(filename.c_str(), _img.cols, _img.rows, _img.channels(), _img.ptr(), 95) != 0;
        }
        else if (extension == ".png")
        {
            return stbi_write_png(filename.c_str(), _img.cols, _img.rows, _img.channels(), _img.ptr(), _img.step_info.linestep) != 0;
        }
        
        return false;
    } // imwrite

    Matrix imdecode(const std::vector<byte>& buffer, int flag = IMREAD_UNCHANGED)
    {
        Matrix img;
        stbi_uc* data;
        int width, height, channels;

        // Set `req_comp` by flag
        int req_comp;
        if (flag == IMREAD_COLOR)
        {
            req_comp = 3;
        }
        else if (flag == IMREAD_GRAYSCALE)
        {
            req_comp = 1;
        }
        else if (flag == IMREAD_UNCHANGED)
        {
            req_comp = 0;
        }
        else
        {
            assert(0 && "Unknown flag");
        }

        // Convert image mode `flag` to stbi_load's `req_comp`
        // Decode image from buffer using stb_image
        data = stbi_load_from_memory(&buffer[0], (int)buffer.size(), &width, &height, &channels, req_comp);
        if (data == NULL)
            goto ret;

        // Copy from buffer
        img.create(width, height, req_comp == 0 ? channels : req_comp, LCV_8U);
        memcpy(img.ptr(), data, (size_t)height * img.step_info.linestep);
        stbi_image_free(data);

        // Change pixel order to save as RGB or RGBA
        if (img.channels() == 3)
            cvtColor(img, img, COLOR_RGB2BGR);
        else if (img.channels() == 4)
            cvtColor(img, img, COLOR_RGBA2BGRA);

        ret:
        return img;
    } // imdecode

    bool imencode(const std::string& ext, const Matrix& img, std::vector<byte>& buf, const std::vector<int>& params = std::vector<int>())
    {
        // Only encode grayscale or color (3ch/4ch) image
        assert(img.channels() == 1 || img.channels() == 3 || img.channels() == 4);

        std::vector<byte> encoded_buffer;
        Matrix _img;

        // Get image file extension from file name to encode image
        std::string extension = ext;
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

        // A callback extending the buffer by stb_image_write
        auto writer_functor = [](void* context, void* data, int size) -> void
        {
            std::vector<byte>& encoded_buffer = *((std::vector<byte>*)context);
            encoded_buffer.insert(encoded_buffer.end(), (byte*)data, (byte*)data + size);
        };

        // Reserve memory
        encoded_buffer.reserve((size_t)img.cols * img.rows * img.elemSize());

        // Change pixel order to save as RGB or RGBA
        if (img.channels() == 3)
            cvtColor(img, _img, COLOR_RGB2BGR);
        else if (img.channels() == 4)
            cvtColor(img, _img, COLOR_BGRA2RGBA);

        // Encode image to buffer to use proper image encoder by the extension
        // `params` is not used yet :(
        if (extension == ".bmp")
        {
            if (stbi_write_bmp_to_func(writer_functor, &encoded_buffer, _img.cols, _img.rows, _img.channels(), _img.ptr()) != 0)
                goto success_ret;
        }
        else if (extension == ".jpg" || extension == ".jpeg")
        {
            if (stbi_write_jpg_to_func(writer_functor, &encoded_buffer, _img.cols, _img.rows, _img.channels(), _img.ptr(), 95) != 0)
                goto success_ret;
        }
        else if (extension == ".png")
        {
            if (stbi_write_png_to_func(writer_functor, &encoded_buffer, _img.cols, _img.rows, _img.channels(), _img.ptr(), _img.step_info.linestep) != 0)
                goto success_ret;
        }

        // When failed
        return false;

        // When succeed
        success_ret:
        encoded_buffer.shrink_to_fit();
        buf = encoded_buffer;
        return true;
    } // imencode
} // namespace lcv
#endif // LCV_IMGCODECS_HPP
