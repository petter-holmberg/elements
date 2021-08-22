#include "catch.hpp"

#include "fill.h"

namespace e = elements;

SCENARIO ("Fill", "[fill]")
{
    SECTION ("Filling an array")
    {
        int x[]{0, 1, 2, 3, 4};

        auto cur = e::fill(x, x + 5, 0);

        REQUIRE (cur == x + 5);

        CHECK(x[0] == 0);
        CHECK(x[1] == 0);
        CHECK(x[2] == 0);
        CHECK(x[3] == 0);
        CHECK(x[4] == 0);
    }

    SECTION ("Filling an array with a sink function")
    {
        int x[]{0, 1, 2, 3, 4};

        auto cur = e::fill(0, 5, 0, [&x](int i, int value) { x[i] = value; });

        REQUIRE (cur == 5);

        CHECK(x[0] == 0);
        CHECK(x[1] == 0);
        CHECK(x[2] == 0);
        CHECK(x[3] == 0);
        CHECK(x[4] == 0);
    }
}
