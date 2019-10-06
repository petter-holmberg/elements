#include "catch.hpp"

#include "count.h"

namespace e = elements;

SCENARIO ("Counting", "[count]")
{
    SECTION ("Counting elements in an array of integers satisfying a predicate")
    {
        int x[]{0, 1, 2, 3, 4};

        auto is_even = [](int i){ return i % 2 == 0; };
        auto is_odd = [](int i){ return i % 2 != 0; };

        REQUIRE (e::count_if(x, x + 5, is_even, 0) == 3);
        REQUIRE (e::count_if(x, x + 5, is_odd, 0) == 2);

        REQUIRE (e::count_if_not(x, x + 5, is_even, 0) == 2);
        REQUIRE (e::count_if_not(x, x + 5, is_odd, 0) == 3);
    }

    SECTION ("Counting elements in an array of integers equalling a value")
    {
        int x[]{0, 1, 0, 0, 4};

        REQUIRE (e::count(x, x + 5, 0) == 3);
        REQUIRE (e::count_not(x, x + 5, 0) == 2);

        REQUIRE (e::count(x, x + 5, 2) == 0);
        REQUIRE (e::count_not(x, x + 5, 2) == 5);
    }
}
