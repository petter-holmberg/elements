#include "catch.hpp"

#include "elements.h"

namespace e = elements;

SCENARIO ("Using coordinate vectors", "[coordinate_vector]")
{
    e::coordinate_vector<int, 2> x{1, 2};
    static_assert(e::Vector_space<decltype(x)>);
    static_assert(e::Sequence<decltype(x)>);

    SECTION ("Vector algebra")
    {
        e::coordinate_vector<int, 2> z;

        REQUIRE (z[0] == 0);
        REQUIRE (z[1] == 0);

        z = -x;

        REQUIRE (z[0] == -1);
        REQUIRE (z[1] == -2);

        e::coordinate_vector<int, 2> y{3, 4};

        z = x + y;

        REQUIRE (z[0] == 4);
        REQUIRE (z[1] == 6);

        z = x - y;

        REQUIRE (z[0] == -2);
        REQUIRE (z[1] == -2);

        z = x * 2;

        REQUIRE (z[0] == 2);
        REQUIRE (z[1] == 4);

        z = y / 2;

        REQUIRE (z[0] == 1);
        REQUIRE (z[1] == 2);
    }
}
