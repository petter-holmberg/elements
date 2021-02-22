#include "catch.hpp"

#include "map.h"

namespace e = elements;

SCENARIO ("Mapping", "[map]")
{
    int x[]{0, 1, 2, 3, 4};
    int y[]{5, 6, 7, 8, 9};

    SECTION ("Unary mapping")
    {
        auto cur = e::map(x, x + 5, y, e::negative);

        REQUIRE (!e::precedes(cur, y + 5));

        CHECK (y[0] == 0);
        CHECK (y[1] == -1);
        CHECK (y[2] == -2);
        CHECK (y[3] == -3);
        CHECK (y[4] == -4);
    }

    SECTION ("Binary mapping")
    {
        auto cur = e::map(x, x + 5, y, y + 0, e::add);

        REQUIRE (!e::precedes(cur, y + 5));

        CHECK (y[0] == 5);
        CHECK (y[1] == 7);
        CHECK (y[2] == 9);
        CHECK (y[3] == 11);
        CHECK (y[4] == 13);
    }
}
