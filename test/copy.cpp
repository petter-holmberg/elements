#include "catch.hpp"

#include "elements.h"

namespace e = elements;

SCENARIO ("Copying", "[copy]")
{
    int x[]{0, 1, 2, 3, 4};
    int y[]{5, 6, 7, 8, 9};

    SECTION ("Copying one array to another")
    {
        auto pos = e::copy(x, x + 5, y);

        REQUIRE(pos == y + 5);
        CHECK (y[0] == 0);
        CHECK (y[1] == 1);
        CHECK (y[2] == 2);
        CHECK (y[3] == 3);
        CHECK (y[4] == 4);
    }

    SECTION ("Copying an array into itelf")
    {
        auto pos = e::copy(x, x + 5, x);

        REQUIRE(pos == x + 5);
        CHECK (x[0] == 0);
        CHECK (x[1] == 1);
        CHECK (x[2] == 2);
        CHECK (x[3] == 3);
        CHECK (x[4] == 4);
    }

    SECTION ("Copying a backward-overlapping sub-range of an array")
    {
        auto pos = e::copy(x + 1, x + 5, x);

        REQUIRE(pos == x + 4);
        CHECK (x[0] == 1);
        CHECK (x[1] == 2);
        CHECK (x[2] == 3);
        CHECK (x[3] == 4);
        CHECK (x[4] == 4);
    }
}
