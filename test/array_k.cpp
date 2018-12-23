#include "catch.hpp"

#include "elements.h"

namespace e = elements;

SCENARIO ("Using array_k", "[array_k]")
{
    e::array_k<int, 5> x{0, 1, 2, 3, 4};
    static_assert(e::Mutable_range<decltype(x)>);
    static_assert(e::Size_value<decltype(x)> == 5);

    SECTION ("Checking elements")
    {
        REQUIRE (!e::is_empty(x));
        REQUIRE (e::size(x) == 5);
        REQUIRE (x[0] == 0);
        REQUIRE (x[1] == 1);
        REQUIRE (x[2] == 2);
        REQUIRE (x[3] == 3);
        REQUIRE (x[4] == 4);
        x[0] = 5;
        x[1] = 4;
        x[2] = 3;
        x[3] = 2;
        x[4] = 1;
        REQUIRE (x[0] == 5);
        REQUIRE (x[1] == 4);
        REQUIRE (x[2] == 3);
        REQUIRE (x[3] == 2);
        REQUIRE (x[4] == 1);
    }

    SECTION ("Comparing arrays")
    {
        auto y = x;
        REQUIRE (x == y);
        REQUIRE (!(x < y));
    }
}
