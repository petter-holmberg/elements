#include "catch.hpp"

#include "elements.h"

namespace e = elements;

SCENARIO ("Using coordinate points", "[coordinate_point]")
{
    e::coordinate_point<int, 2> x{1, 2};
    static_assert(e::Affine_space<decltype(x)>);
    static_assert(e::Sequence<decltype(x)>);

    SECTION ("Affine transformations")
    {
        e::coordinate_point<int, 2> z;

        REQUIRE (z[0] == 0);
        REQUIRE (z[1] == 0);

        e::Difference_type<decltype(x)> v = x - z;

        REQUIRE (v[0] == 1);
        REQUIRE (v[1] == 2);

        x = x + v;

        REQUIRE (x[0] == 2);
        REQUIRE (x[1] == 4);

        x = v + x;

        REQUIRE (x[0] == 3);
        REQUIRE (x[1] == 6);

        x = x - v;

        REQUIRE (x[0] == 2);
        REQUIRE (x[1] == 4);
    }
}
