#pragma once
#ifndef LCV_IMGCODECS_IMGCODECS_HPP
#define LCV_IMGCODECS_IMGCODECS_HPP
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>

#include "liteCV/core/lcvdef.hpp"
#include "liteCV/core/matrix.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "enums.hpp"

namespace lcv
{
    Matrix imread(const std::string& filename, int flag = IMREAD_COLOR)
    {
        Matrix img;
        stbi_uc* data;
        int width, height, channels;

        // Load image from file using stb_image
        // `flag` is not used yet :(
        data = stbi_load(filename.c_str(), &width, &height, &channels, 3);
        if (data == nullptr)
            return img;

        // Copy from buffer
        img.create(width, height);
        memcpy(img.ptr(), data, (size_t)width * height * 3);
        stbi_image_free(data);

        return img;
    }

    bool imwrite(const std::string& filename, const Matrix& img, const std::vector<int>& params = std::vector< int >())
    {
        // `std::filesystem` only can use in C++17 :(
        const std::filesystem::path path = filename;

        // Get image file extension from file name to encode image
        std::string extension = path.extension().string();
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

        // Write image to file to use proper image encoder by the extension
        // `params` is not used yet :(
        if (extension == ".bmp")
        {
            return stbi_write_bmp(filename.c_str(), img.cols, img.rows, img.type.nchans, img.cptr()) != 0;
        }
        else if (extension == ".jpg" || extension == ".jpeg")
        {
            return stbi_write_jpg(filename.c_str(), img.cols, img.rows, img.type.nchans, img.cptr(), 95) != 0;
        }
        else if (extension == ".png")
        {
            return stbi_write_png(filename.c_str(), img.cols, img.rows, img.type.nchans, img.cptr(), img.cols * img.type.nchans) != 0;
        }
        
        return false;
    }
} // namespace lcv
#endif // LCV_IMGCODECS_IMGCODECS_HPP
