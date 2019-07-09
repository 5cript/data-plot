#include "../cartesian_world.hpp"

#include <iostream>
#include <random>
#include <cmath>

int main()
{
    using namespace DataPlot;

    int Width = 800;
    int Height = 600;

    Cairo::Surface surface{Width, Height};
    Cairo::DrawContext ctx{&surface};

    ctx.fill(Cairo::Colors::White);

    CartesianWorld world{&ctx, Width, Height, Cairo::Colors::White};
    //world.changeView(-10, 2000, 3000, 6000);
    world.changeView(-3000, -2000, -6000, -3000);

    DataSet<double> points;
    DataSet<double> points2;

    std::random_device r;
    std::seed_seq seed2{r(), r(), r(), r(), r(), r(), r(), r()};
    std::mt19937 gen(seed2);

    std::uniform_real_distribution<double> dst{0., 600.};

    /*
    const double abtastRate = 0.02;
    for (int i = 0; i != world.xMax()*abtastRate; ++i)
    {
        auto sin = (world.yMax() / 2.) * std::sin(static_cast <double>(i)/abtastRate);

        points.insert(static_cast <double>(i)/abtastRate, sin);
    }
    */


    PointOptions point;
    point.style = PointDrawStyle::Cross;
    PointOptions point2;
    point2.style = PointDrawStyle::Dot;
    point2.color1 = Cairo::Colors::Red;
    point2.color2 = Cairo::Colors::Red;

    //world.renderFunction([&](double x){return points.max();}, {}, Cairo::Colors::Red);
    //world.renderFunction([&](double x){return points.min();}, {}, Cairo::Colors::Red);

    //world.renderPath(points, Cairo::Colors::Magenta);
    //world.renderScatterPlot(points, point);
    world.renderFunction([](double x){return 5.*std::sin(x);}, {false, 1000}, Cairo::Colors::Blue);
    //world.renderPath(points2, Cairo::Colors::Black);

    AxisOptions xOptions, yOptions;
    yOptions.fontSize = 15;
    xOptions.fontSize = 15;
    xOptions.dashCount = (-world.xMin() + world.xMax()) / 100.;
    yOptions.dashCount = (-world.yMin() + world.yMax()) / 100.;
    world.renderLinearAxes(xOptions, yOptions);

    surface.saveToFile("data.png");

    return 0;
}
