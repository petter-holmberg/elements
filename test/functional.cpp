#include "catch.hpp"

#include "copy.h"
#include "functional.h"

namespace e = elements;

SCENARIO ("Using filter position", "[filter_position]")
{
    int x[]{0, 1, 2, 3, 4};
    int y[]{5, 6, 7, 8, 9};

    auto is_even = [](int i){ return i % 2 == 0; };

    SECTION ("Filtering with sink")
    {
        auto pos = e::copy(x, x + 5, e::filter_sink{is_even}(y + 0));
        REQUIRE(!precedes(pos, y + 3));
        CHECK (y[0] == 0);
        CHECK (y[1] == 2);
        CHECK (y[2] == 4);
        CHECK (y[3] == 8);
        CHECK (y[4] == 9);
    }
}

SCENARIO ("Using map position", "[map_position]")
{
    int x[]{0, 1, 2, 3, 4};
    int y[]{5, 6, 7, 8, 9};

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
