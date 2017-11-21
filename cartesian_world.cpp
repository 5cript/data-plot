#include "cartesian_world.hpp"

#include <cairo-wrap/path.hpp>
#include <cairo-wrap/line.hpp>
#include <cairo-wrap/arrow.hpp>

#include <iostream>

namespace DataPlot
{
//#####################################################################################################################
    CartesianWorld::CartesianWorld(Cairo::DrawContext* ctx, int width, int height)
        : ctx_{ctx}
        , width_{width}
        , height_{height}
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
    void CartesianWorld::renderSet(DataSet <value_type> const& set, Color color)
    {
        if (set.empty())
            return;

        Cairo::Path path{ctx_, toWorldX(std::begin(set)->first), toWorldY(std::begin(set)->second)};
        path.start();
        for (auto i = std::begin(set); i < std::end(set); ++i)
        {
            /*
            std::cout << "---\n";
            std::cout << toWorldX(i->first) << "\n"
                  << toWorldY(i->second)  << "\n";
            */

            path.lineTo(
                toWorldX(i->first),
                toWorldY(i->second)
            );
        }
        path.draw(color, {});
    }
//---------------------------------------------------------------------------------------------------------------------
    CartesianWorld::value_type CartesianWorld::toWorldX(value_type value) const
    {
        auto axisAccomodation = 0.;
        if (xMin_ * static_cast <double>(width_) / (xMax_ - xMin_) <= axisWidth)
            axisAccomodation = axisWidth;
        auto scaler = static_cast <double>(width_ - axisAccomodation) / (xMax_ - xMin_);
        auto translated = -xMin_ * scaler + value * scaler;
        return translated;
    }
//---------------------------------------------------------------------------------------------------------------------
    CartesianWorld::value_type CartesianWorld::toWorldY(value_type value) const
    {
        auto axisAccomodation = 0.;
        if (yMin_ * static_cast <double>(width_) / (yMax_ - yMin_) <= axisWidth)
            axisAccomodation = axisWidth;
        auto scaler = static_cast <double>(width_ - axisAccomodation) / (yMax_ - yMin_);
        auto translated = height_ - (-yMin_ * scaler + value * scaler);
        return translated;
    }
//---------------------------------------------------------------------------------------------------------------------
    void CartesianWorld::renderLinearAxes(Color color, int dashStep)
    {
        std::pair <value_type, value_type> origin = {
            toWorldX(0),
            toWorldY(0)
        };

        Cairo::Arrow xAxis{ctx_, origin.first - axisWidth / 2, origin.second - axisWidth / 2, toWorldX(xMax_), origin.second - axisWidth / 2};
        xAxis.draw(color, color);

        Cairo::Arrow yAxis{ctx_, origin.first - axisWidth / 2, origin.second - axisWidth / 2, origin.first - axisWidth / 2, 0};
        yAxis.draw(color, color);

        Cairo::Line xAxisPartTwo{ctx_, origin.first - axisWidth / 2, origin.second - axisWidth / 2, 0, origin.second-axisWidth / 2};
        xAxisPartTwo.draw(color, color);

        Cairo::Line yAxisPartTwo{ctx_, origin.first - axisWidth / 2, origin.second - axisWidth / 2, origin.first - axisWidth / 2, static_cast <double> (height_)};
        yAxisPartTwo.draw(color, color);
    }
//---------------------------------------------------------------------------------------------------------------------
    void CartesianWorld::renderFunction(Function <value_type> const& function, Color color)
    {
    }
//#####################################################################################################################
}
