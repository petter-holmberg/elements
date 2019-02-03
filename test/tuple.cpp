#include "catch.hpp"

#include "elements.h"

namespace e = elements;

SCENARIO ("Usig tuples", "[tuple]")
{
    SECTION ("tuples with zero elements")
    {
        e::tuple<> x;
    }

    SECTION ("tuples with one element")
    {
        e::tuple<int> x(0);
        e::tuple<double> y(0.0);
    }

    SECTION ("tuples with two elements")
    {
        e::tuple<int, int> x(0, 0);
        e::tuple<int, double> y(0, 0);
    }

    SECTION ("tuples with three elements")
    {
        e::tuple<int, int, int> x(0, 0, 0);
        e::tuple<int, double, double> y(0, 0, 0);
    }
}
