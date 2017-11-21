#pragma once

#include "dataset.hpp"
#include "function.hpp"
#include "color.hpp"

#include "cairo-wrap/cairo_wrap.hpp"

namespace DataPlot
{
    class CartesianWorld
    {
    public:
        using value_type = double;
        static constexpr auto axisWidth = 8;

        CartesianWorld(Cairo::DrawContext* ctx, int width, int height);

        /**
         *  Changes the borders of the coordinate system.
         */
        void changeView(double xMin, double xMax, double yMin, double yMax);

        /**
         *  @param set A set of values to render.
         */
        void renderSet(DataSet <value_type> const& set, Color color);

        /**
         *  @param function A function to render.
         */
        void renderFunction(Function <value_type> const& function, Color color);

        /**
         *  Render axes.
         *  @param dashStep the amount of steps to put dashes.
         */
        void renderLinearAxes(Color color, int dashStep = 10);

    private:
        /**
         *  Transforms a view value to an image coordinate.
         */
        value_type toWorldX(value_type value) const;
        value_type toWorldY(value_type value) const;

    private:
        Cairo::DrawContext* ctx_;

        int width_;
        int height_;

        double yMin_;
        double yMax_;
        double xMin_;
        double xMax_;
    };
}
