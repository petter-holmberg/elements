#include "catch.hpp"

#include "reverse.h"

namespace e = elements;

SCENARIO ("Using swap", "[swap]")
{
    int x[]{0, 1, 2, 3, 4};
    int y[]{5, 6, 7, 8, 9};

    SECTION ("Reverse swapping")
    {
        auto first_x = e::bidirectional_position{x};
        auto limit_x = e::bidirectional_position{x + 5};
        auto first_y = e::bidirectional_position{y};
        auto limit_y = e::bidirectional_position{y + 5};

        SECTION ("Swapping one range with itself")
        {
            auto [pos0, pos1] = e::reverse_swap(first_x, limit_x, first_x, limit_x);

            REQUIRE(pos0 == first_x);
            REQUIRE(pos1 == limit_x);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Swapping two ranges")
        {
            auto [pos0, pos1] = e::reverse_swap(first_x, limit_x, first_y, limit_y);

            REQUIRE(pos0 == first_x);
            REQUIRE(pos1 == limit_y);

            CHECK (x[0] == 9);
            CHECK (x[1] == 8);
            CHECK (x[2] == 7);
            CHECK (x[3] == 6);
            CHECK (x[4] == 5);

            CHECK (y[0] == 4);
            CHECK (y[1] == 3);
            CHECK (y[2] == 2);
            CHECK (y[3] == 1);
            CHECK (y[4] == 0);
        }
    }
}

