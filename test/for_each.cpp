#include "catch.hpp"

#include "for_each.h"

namespace e = elements;

SCENARIO ("For each", "[for_each]")
{
    SECTION ("Summing the integers in an array")
    {
        int x[]{0, 1, 2, 3, 4};

        int sum = 0;
        e::for_each(x, x + 5, [&sum](int a){ sum += a; });

        REQUIRE (sum == 10);
    }

    SECTION ("Summing the first three integers in an array")
    {
        int x[]{0, 1, 2, 3, 4};

        int sum = 0;
        e::for_each(x, x + 3, [&sum](int a){ sum += a; });

        REQUIRE (sum == 3);
    }
}

SCENARIO ("For each n", "[for_each_n]")
{
    SECTION ("Summing the integers in an array")
    {
        int x[]{0, 1, 2, 3, 4};

        int sum = 0;
        e::for_each_n(e::counted_cursor{x + 0, 5}, 5, [&sum](int a){ sum += a; });

        REQUIRE (sum == 10);
    }
}
