#include "cartesian_world.hpp"

#include <iostream>
#include <random>
#include <cmath>

int main()
{
    using namespace DataPlot;

    Cairo::Surface surface{800, 600};
    Cairo::DrawContext ctx{&surface};

    ctx.fill(Cairo::Colors::White);

    CartesianWorld world{&ctx, 800, 600};
    world.changeView(-200, 1600, 0, 1200);

    DataSet<double> points;
    DataSet<double> points2;

    std::random_device r;
    std::seed_seq seed2{r(), r(), r(), r(), r(), r(), r(), r()};
    std::mt19937 gen(seed2);

    std::uniform_real_distribution<double> dst{0., 600.};

    auto averaged = 400.;
    for (int i = 0; i != 500; ++i)
        averaged = (dst(gen) + averaged) / 2;

    for (int i = 0; i != 1600; ++i)
    {
        averaged = (1 * dst(gen) + 9 * averaged) / 10;
        auto sin = 300. + 100. * std::sin(3.14159265359 * i/180.);
        points.insert(static_cast <double> (i), averaged + sin - 300.);

        points2.insert(i, sin);
    }

    world.renderSet(points, Cairo::Colors::Magenta);
    world.renderSet(points2, Cairo::Colors::Black);

    world.renderLinearAxes(Cairo::Colors::Black);

    surface.saveToFile("data.png");

    return 0;
}
