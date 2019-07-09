#pragma once

#include "color.hpp"

namespace DataPlot
{
    enum class AxisValueWriteStrategy
    {
        None,
        Bounds,
        ByDash
    };

    struct AxisOptions
    {
        Color lineColor = Cairo::Colors::Black;
        Color dashColor = Cairo::Colors::Black;
        Color arrowColor = Cairo::Colors::Black;
        Color cutColor = Cairo::Colors::Black;
        int axisWidth = 8;
        int axisCutDistance = 8;
        int dashCount = 20;
        Color fontColor = Cairo::Colors::Black;
        int fontSize = 10;
        int axisNumberPrecision = 0;
        AxisValueWriteStrategy valueWriting = AxisValueWriteStrategy::Bounds;
    };
}
