#pragma once
#ifndef LCV_CORE_MATRIX_FACTORY_HPP
#define LCV_CORE_MATRIX_FACTORY_HPP
#include <string>
#include <regex>

namespace lcv
{
    void parse_channels(const std::string& channels, int& nchans, int& depth)
    {
        std::regex re("(\\d+)([f|i|u])c(\\d+)");
        std::smatch match;
        if (std::regex_match(channels, match, re))
        {
            std::string _dtype;
            int _nchans, _depth;
            
            _depth = std::stoi(match[1]);
            _nchans = std::stoi(match[3]);

            // Depth must be greater than zero and power of two
            // A number of channels must be greater than zero
            assert(_depth > 0 && !(_depth & (_depth - 1)));
            assert(_nchans > 0);

            nchans = _nchans;
            depth = _depth;
        }
    }
} // namespace lcv
#endif // LCV_CORE_MATRIX_FACTORY_HPP
