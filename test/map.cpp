#include "catch.hpp"

#include "elements.h"

namespace e = elements;

SCENARIO ("Mapping", "[map]")
{
    int x[]{0, 1, 2, 3, 4};
    int y[]{5, 6, 7, 8, 9};

    SECTION ("Unary mapping")
    {
        auto pos = e::map(x, x + 5, y, e::negative<int>{});

        REQUIRE(pos == y + 5);
        CHECK (y[0] == 0);
        CHECK (y[1] == -1);
        CHECK (y[2] == -2);
        CHECK (y[3] == -3);
        CHECK (y[4] == -4);
    }

    SECTION ("Binary mapping")
    {
        auto pos = e::map(x, x + 5, y, y + 0, e::sum<int>{});

        REQUIRE(pos == y + 5);
        CHECK (y[0] == 5);
        CHECK (y[1] == 7);
        CHECK (y[2] == 9);
        CHECK (y[3] == 11);
        CHECK (y[4] == 13);
    }

    SECTION ("Mapping with sink")
    {
        auto pos = e::copy(x, x + 5, e::map_sink{e::negative<int>{}}(y + 0));
        REQUIRE(!precedes(pos, y + 5));
        CHECK (y[0] == 0);
        CHECK (y[1] == -1);
        CHECK (y[2] == -2);
        CHECK (y[3] == -3);
        CHECK (y[4] == -4);
    }
}
