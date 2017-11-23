#pragma once

#include "dataset.hpp"
#include "function.hpp"
#include "color.hpp"
#include "axis_options.hpp"
#include "point_options.hpp"
#include "function_render_options.hpp"

#include <cairo-wrap/cairo_wrap.hpp>

namespace DataPlot
{
    class CartesianWorld
    {
    public:
        using value_type = double;
        static constexpr auto axisWidth = 8;
        static constexpr auto axisCutDistance = 8;

        CartesianWorld(Cairo::DrawContext* ctx, int width, int height, Color background);

        /**
         *  Changes the borders of the coordinate system.
         */
        void changeView(double xMin, double xMax, double yMin, double yMax);

        /**
         *  @param set A set of values to render.
         */
        void renderPath(DataSet <value_type> const& set, Color color);

        /**
         *  Render data set as scatter plot
         */
        void renderScatterPlot(DataSet <value_type> const& set, PointOptions const& opts);

        /**
         *  Draws the function as a path.
         *  @param function A function to render.
         */
        void renderFunction(std::function <value_type(value_type)> const& function, FunctionRenderOptions const& renderOptions, Color color);

        /**
         *  Draws the function as a series of points.
         *  @param function A function to draw.
         */
        void renderFunctionPoints(std::function <value_type(value_type)> const& function, FunctionRenderOptions const& renderOptions, PointOptions const& point);

        /**
         *  Render axes.
         *  @param dashStep the amount of steps to put dashes.
         */
        void renderLinearAxes(AxisOptions const& xOptions, AxisOptions const& yOptions);

        /**
         *  Render a single point into the coordinate system.
         */
        void renderPoint(value_type x, value_type y, PointOptions const& point);

        value_type yMax() const
        {
            return yMax_;
        }
        value_type yMin() const
        {
            return yMin_;
        }
        value_type xMax() const
        {
            return xMax_;
        }
        value_type xMin() const
        {
            return xMin_;
        }

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

        Color background_;

        double yMin_;
        double yMax_;
        double xMin_;
        double xMax_;
    };
}
