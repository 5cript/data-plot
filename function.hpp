#pragma once

#include "dataset.hpp"

#include <functional>

namespace DataPlot
{
    template <typename T>
    class Function
    {
    public:
        constexpr Function(std::function <T(T)> func)
            : func_{func}
        {
        }

        constexpr T operator()(T x) const
        {
            return func_(x);
        }

    private:
        std::function <T(T)> func_;
    };
}
