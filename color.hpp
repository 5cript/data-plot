#pragma once

#include <cstdint>

namespace DataPlot
{
    struct Color
    {
        union {
            struct {
                uint8_t A : 8;
                uint8_t R : 8;
                uint8_t G : 8;
                uint8_t B : 8;
            };
            uint32_t value;
        };

        Color() = default;
        ~Color() = default;
        Color(Color const&) = default;
        Color(Color&&) = default;
        Color& operator=(Color const&) = default;
        Color& operator=(Color&&) = default;
        Color(uint32_t value) : value{value}{}
    };
}
