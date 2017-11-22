#pragma once

#include "color.hpp"

namespace DataPlot
{
    enum PointDrawStyle
    {
        Circle,
        X,
        Cross,
        Dot
    };

    struct PointOptions
    {
        PointDrawStyle style = PointDrawStyle::X;
        Color color1 = Cairo::Colors::Black;
        Color color2 = Cairo::Colors::Black;
        double scale = 3.0;
    };
}
