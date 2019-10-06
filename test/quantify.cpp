#include "catch.hpp"

#include "quantify.h"

namespace e = elements;

SCENARIO ("Quantifiers", "[quantify]")
{
    auto is_even = [](int i){ return i % 2 == 0; };
    auto is_odd = [](int i){ return i % 2 != 0; };

    SECTION ("Array of even integers")
    {
        int x[]{0, 2, 4, 6, 8};

        REQUIRE (e::each_of(x, x + 5, is_even));
        REQUIRE (!e::each_of(x, x+ 5, is_odd));

        REQUIRE (!e::any_not_of(x, x + 5, is_even));
        REQUIRE (e::any_not_of(x, x + 5, is_odd));

        REQUIRE (!e::none_of(x, x + 5, is_even));
        REQUIRE (e::none_of(x, x + 5, is_odd));

        REQUIRE (e::any_of(x, x + 5, is_even));
        REQUIRE (!e::any_of(x, x + 5, is_odd));
    }

    GIVEN ("Array of even and odd integers")
    {
        int x[]{0, 1, 2, 3, 4};

        REQUIRE (!e::each_of(x, x + 5, is_even));
        REQUIRE (!e::each_of(x, x + 5, is_odd));

        REQUIRE (e::any_not_of(x, x + 5, is_even));
        REQUIRE (e::any_not_of(x, x + 5, is_odd));

        REQUIRE (!e::none_of(x, x + 5, is_even));
        REQUIRE (!e::none_of(x, x + 5, is_odd));

        REQUIRE (e::any_of(x, x + 5, is_even));
        REQUIRE (e::any_of(x, x + 5, is_odd));
    }
}
