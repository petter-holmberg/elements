#include "catch.hpp"

#include "reduce.h"

namespace e = elements;

SCENARIO ("Reduction", "[reduce]")
{
    int x[]{0, 1, 2, 3, 4};

    SECTION ("Reduction of integer range")
    {
        REQUIRE (e::reduce_nonempty(1, 6, e::multiply) == 120);
    }

    SECTION ("Reduction of array")
    {
        REQUIRE (e::reduce(x, x, e::add_op<int>{}, 5) == 5);

        REQUIRE (e::reduce_nonempty(x, x + 5, e::add) == 10);
    }
}

SCENARIO ("Balanced reduction", "[reduce]")
{
    int x[]{0, 1, 2, 3, 4};

    SECTION ("Balanced reduction of array")
    {
        REQUIRE(e::reduce_balanced(x, x + 5, e::add, 0) == 10);
    }
}
