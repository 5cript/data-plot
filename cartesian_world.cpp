#include "cartesian_world.hpp"

#include <cairo-wrap/path.hpp>
#include <cairo-wrap/line.hpp>
#include <cairo-wrap/arrow.hpp>
#include <cairo-wrap/rectangle.hpp>
#include <cairo-wrap/arc.hpp>

#include <iostream>
#include <memory>
#include <sstream>

namespace DataPlot
{
//#####################################################################################################################
    CartesianWorld::CartesianWorld(Cairo::DrawContext* ctx, int width, int height, Color background)
        : ctx_{ctx}
        , width_{width}
        , height_{height}
        , background_{background}
        , yMin_{0}
        , yMax_{0}
        , xMin_{0}
        , xMax_{0}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void CartesianWorld::changeView(double xMin, double xMax, double yMin, double yMax)
    {
        yMin_ = yMin;
        yMax_ = yMax;
        xMin_ = xMin;
        xMax_ = xMax;
    }
//---------------------------------------------------------------------------------------------------------------------
    void CartesianWorld::renderPath(DataSet <value_type> const& set, Color color)
    {
        if (set.empty())
            return;

        Cairo::Path path{ctx_, toWorldX(std::begin(set)->first), toWorldY(std::begin(set)->second)};
        path.start();
        for (auto i = std::begin(set); i < std::end(set); ++i)
        {
            path.lineTo(
                toWorldX(i->first),
                toWorldY(i->second)
            );
        }
        path.draw(color, {});
    }
//---------------------------------------------------------------------------------------------------------------------
    void CartesianWorld::renderScatterPlot(DataSet <value_type> const& set, PointOptions const& opts)
    {
        if (set.empty())
            return;

        for (auto const& i : set)
        {
            renderPoint(i.first, i.second, opts);
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    CartesianWorld::value_type CartesianWorld::toWorldX(value_type value) const
    {
        auto scaler = static_cast <double>(width_) / (xMax_ - xMin_);
        auto translated = -xMin_ * scaler + value * scaler;
        return translated;
    }
//---------------------------------------------------------------------------------------------------------------------
    CartesianWorld::value_type CartesianWorld::toWorldY(value_type value) const
    {
        auto scaler = static_cast <double>(height_) / (yMax_ - yMin_);
        auto translated = height_ - (-yMin_ * scaler + value * scaler);
        return translated;
    }
//---------------------------------------------------------------------------------------------------------------------
    void CartesianWorld::renderLinearAxes(AxisOptions const& xOptions, AxisOptions const& yOptions)
    {
        auto toString = [](auto value, int precision)
        {
            std::stringstream sstr;
            sstr.precision(precision);
            sstr << std::fixed << value;
            return sstr.str();
        };

        std::pair <value_type, value_type> origin = {
            toWorldX(0),
            toWorldY(0)
        };

        // x-Axis is out of world (bottom)
        if (origin.second > height_/* - xOptions.axisWidth*/)
            origin.second = height_/* - xOptions.axisWidth*/;

        // x-Axis is out of world (top)
        if (origin.second < 0)
            origin.second = 0/* + xOptions.axisWidth*/;

        // y-Axis is out of world (left)
        if (origin.first < 0)
            origin.first = 0 + yOptions.axisWidth;

        // y-Axis is out of world (right)
        if (origin.first > width_ - yOptions.axisWidth)
            origin.first = width_ - yOptions.axisWidth;

        Cairo::Arrow xAxis{ctx_, origin.first, origin.second, toWorldX(xMax_), origin.second};
        xAxis.draw(xOptions.lineColor, xOptions.arrowColor);

        Cairo::Arrow yAxis{ctx_, origin.first, origin.second, origin.first, 0};
        yAxis.draw(yOptions.lineColor, yOptions.arrowColor);

        Cairo::Line xAxisPartTwo{ctx_, origin.first, origin.second, 0, origin.second};
        xAxisPartTwo.draw(xOptions.lineColor);

        Cairo::Line yAxisPartTwo{ctx_, origin.first, origin.second, origin.first, static_cast <double> (height_)};
        yAxisPartTwo.draw(yOptions.lineColor);

        // x-Axis is out of world (bottom)
        if (yMin_ > 0)
        {
            Cairo::Line cut1{
                ctx_,
                origin.first + yOptions.axisCutDistance,
                origin.second - yOptions.axisWidth -3,
                origin.first - yOptions.axisCutDistance,
                origin.second - yOptions.axisWidth + 3
            };
            Cairo::Line cut2{
                ctx_,
                origin.first + yOptions.axisCutDistance,
                origin.second - yOptions.axisWidth -6,
                origin.first - yOptions.axisCutDistance,
                origin.second - yOptions.axisWidth
            };
            Cairo::Line cut3{
                ctx_,
                origin.first + yOptions.axisCutDistance,
                origin.second - yOptions.axisWidth -4,
                origin.first - yOptions.axisCutDistance,
                origin.second - yOptions.axisWidth + 2
            };
            cut1.draw(yOptions.cutColor);
            cut2.draw(yOptions.cutColor);
            cut3.draw(background_);

            // write into cut
            if (yOptions.fontSize > 0 && xOptions.valueWriting != AxisValueWriteStrategy::None)
            {
                auto yPosition = origin.second - xOptions.fontSize - 8;
                if (origin.second < static_cast <int> (xOptions.fontSize * 1.5))
                    yPosition = origin.second + 8;

                Cairo::Text text{
                    ctx_,
                    origin.first + yOptions.axisCutDistance,
                    yPosition,
                    toString(yMin_, xOptions.axisNumberPrecision),
                    Cairo::Font{"Arial", static_cast <double> (xOptions.fontSize)}
                };
                text.draw(xOptions.fontColor);
            }
        }

        if (xMin_ > 0)
        {
            Cairo::Line cut1{
                ctx_,
                origin.first + xOptions.axisCutDistance + 3,
                origin.second - xOptions.axisWidth, origin.first + xOptions.axisCutDistance - 3,
                origin.second + xOptions.axisWidth
            };
            Cairo::Line cut2{
                ctx_,
                origin.first + xOptions.axisCutDistance + 6,
                origin.second - xOptions.axisWidth,
                origin.first + xOptions.axisCutDistance,
                origin.second + xOptions.axisWidth
            };
            Cairo::Line cut3{
                ctx_,
                origin.first + xOptions.axisCutDistance + 4,
                origin.second - xOptions.axisWidth,
                origin.first + xOptions.axisCutDistance - 2,
                origin.second + xOptions.axisWidth
            };
            cut1.draw(xOptions.cutColor);
            cut2.draw(xOptions.cutColor);
            cut3.draw(background_);
        }

        // x-dashes
        for (int i = -xOptions.dashCount; i != xOptions.dashCount; ++i)
        {
            auto x = origin.first + toWorldX(xMin_ + static_cast <double> (i) * (-xMin_ + xMax_) / xOptions.dashCount);
            Cairo::Line dash{ctx_, x, origin.second + axisWidth / 2, x, origin.second - axisWidth / 2};
            dash.draw(xOptions.dashColor);
        }

        // y-dashes
        for (int i = -yOptions.dashCount; i != yOptions.dashCount; ++i)
        {
            auto y = toWorldY(yMin_ + static_cast <double> (i) * static_cast <double>(-yMin_ + yMax_) / static_cast <double> (yOptions.dashCount));
            Cairo::Line dash{ctx_, origin.first - axisWidth / 2, y, origin.first + axisWidth / 2, y};
            dash.draw(yOptions.dashColor);
        }

        // write max to axis (x)
        if (xOptions.fontSize > 0 && xOptions.valueWriting != AxisValueWriteStrategy::None)
        {
            auto yPosition = origin.second - xOptions.fontSize - 8;
            if (origin.second < static_cast <int> (xOptions.fontSize * 1.5))
                yPosition = origin.second + 8;

            double fontWidth;
            {
                Cairo::Surface surface{width_, height_};
                Cairo::DrawContext dummy{&surface};
                auto bounds = Cairo::Text{
                    &dummy,
                    0,
                    0,
                    toString(xMax_, xOptions.axisNumberPrecision),
                    Cairo::Font{"Arial", static_cast <double> (xOptions.fontSize)}
                }.calculateBounds(xOptions.fontColor);
                fontWidth = bounds.getWidth();
            }

            Cairo::Text text{
                ctx_,
                toWorldX(xMax_) - fontWidth,
                yPosition,
                toString(xMax_, xOptions.axisNumberPrecision),
                Cairo::Font{"Arial", static_cast <double> (xOptions.fontSize)}
            };
            text.draw(xOptions.fontColor);
        }

        // write max to axis (y)
        if (yOptions.fontSize > 0 && xOptions.valueWriting != AxisValueWriteStrategy::None)
        {
            auto xPosition = origin.first + 8;
            if (width_ - origin.first < 100)
            {
                Cairo::Surface surface{width_, height_};
                Cairo::DrawContext dummy{&surface};
                auto bounds = Cairo::Text{
                    &dummy,
                    0,
                    0,
                    toString(yMax_, yOptions.axisNumberPrecision),
                    Cairo::Font{"Arial", static_cast <double> (yOptions.fontSize)}
                }.calculateBounds(yOptions.fontColor);
                xPosition = origin.first - bounds.getWidth() - 8;
            }

            Cairo::Text text{
                ctx_,
                xPosition,
                1,
                toString(yMax_, yOptions.axisNumberPrecision),
                Cairo::Font{"Arial", static_cast <double> (yOptions.fontSize)}
            };
            text.draw(yOptions.fontColor);
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    void CartesianWorld::renderPoint(value_type x, value_type y, PointOptions const& point)
    {
        auto wx = toWorldX(x);
        auto wy = toWorldY(y);

        switch (point.style)
        {
            case PointDrawStyle::Dot:
                Cairo::Rectangle{ctx_, wx - point.scale / 2., wy - point.scale / 2., point.scale, point.scale}.draw(point.color1, point.color2);
                break;
            case PointDrawStyle::X:
                Cairo::Line{ctx_, wx - point.scale, wy + point.scale, wx + point.scale, wy - point.scale}.draw(point.color1, point.color2);
                Cairo::Line{ctx_, wx - point.scale, wy - point.scale, wx + point.scale, wy + point.scale}.draw(point.color1, point.color2);
                break;
            case PointDrawStyle::Cross:
                Cairo::Line{ctx_, wx - point.scale, wy, wx + point.scale, wy}.draw(point.color1, point.color2);
                Cairo::Line{ctx_, wx, wy - point.scale, wx, wy + point.scale}.draw(point.color1, point.color2);
                break;
            case PointDrawStyle::Circle:
                Cairo::Arc{ctx_, wx, wy, point.scale}.draw(point.color1, background_);
                break;
            default: break;
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    void CartesianWorld::renderFunction(
        std::function <value_type(value_type)> const& function,
        FunctionRenderOptions const& renderOptions,
        Color color
    )
    {
        double sampleCount = width_;
        if (!renderOptions.autoSampler)
            sampleCount = renderOptions.sampleCount;

        Cairo::Path path{ctx_, toWorldX(xMin_), toWorldY(function(xMin_))};
        path.start();
        double stepDistance = (std::abs(xMin_) + std::abs(xMax_)) / sampleCount;
        for (auto i = xMin_ + stepDistance; i <= xMax_; i += stepDistance)
        {
            path.lineTo(
                toWorldX(i),
                toWorldY(function(i))
            );
        }
        path.draw(color, {});
    }
//---------------------------------------------------------------------------------------------------------------------
    void CartesianWorld::renderFunctionPoints(
        std::function <value_type(value_type)> const& function,
        FunctionRenderOptions const& renderOptions,
        PointOptions const& point
    )
    {
        double sampleCount = width_ / point.scale;
        if (!renderOptions.autoSampler)
            sampleCount = renderOptions.sampleCount;
        double stepDistance = (-xMin_ + xMax_) / sampleCount;

        for (auto i = xMin_ + stepDistance; i <= xMax_; i += stepDistance)
        {
            renderPoint(i, function(i), point);
        }
    }
//#####################################################################################################################
}
