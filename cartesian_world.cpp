#include "cartesian_world.hpp"

#include <cairo-wrap/path.hpp>

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
    void CartesianWorld::changeView(double yMin, double yMax, double xMin, double xMax)
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

        std::cout << fixX(std::begin(set)->first) << "\n"
                  << fixY(std::begin(set)->second)  << "\n";

        Cairo::Path path{ctx_, fixX(std::begin(set)->first), fixY(std::begin(set)->second)};
        path.start();
        for (auto i = std::begin(set); i < std::end(set); ++i)
        {
            std::cout << "---\n";
            std::cout << fixX(i->first) << "\n"
                  << fixY(i->second)  << "\n";

            path.lineTo(
                fixX(i->first),
                fixY(i->second)
            );
        }
        path.draw(Cairo::Colors::Red, {});
    }
//---------------------------------------------------------------------------------------------------------------------
    CartesianWorld::value_type CartesianWorld::fixX(value_type value) const
    {
        return -xMin_ + value * static_cast <double>(width_) / (xMax_ - xMin_);
    }
//---------------------------------------------------------------------------------------------------------------------
    CartesianWorld::value_type CartesianWorld::fixY(value_type value) const
    {
        return height_  - value * static_cast <double>(height_) / (yMax_ - yMin_);
    }
//---------------------------------------------------------------------------------------------------------------------
    void CartesianWorld::renderFunction(Function <value_type> const& function, Color color)
    {
    }
//#####################################################################################################################
}
