#include "catch.hpp"

#include "elements.h"

namespace e = elements;

SCENARIO ("Reduction", "[reduce]")
{
    int x[]{0, 1, 2, 3, 4};

    SECTION ("Reduction of integer range")
    {
        REQUIRE (e::reduce_nonempty(1, 6, e::product<int>{}) == 120);
    }

    SECTION ("Reduction of array")
    {
        REQUIRE (e::reduce(x, x, e::sum<int>{}, 5) == 5);

        REQUIRE (e::reduce_nonempty(x, x + 5, e::sum<int>{}) == 10);
    }
}
