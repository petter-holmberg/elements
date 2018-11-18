#include "catch.hpp"

#include "elements.h"

namespace e = elements;

SCENARIO ("Using access functions", "[position]")
{
    SECTION ("Loadable")
    {
        int x = 0;
        REQUIRE (e::load(x) == x);

        auto y = e::pointer_to(x);
        REQUIRE(e::load(y) == x);
    }

    SECTION ("Storable")
    {
        int x = 0;
        e::store(x, 1);
        REQUIRE (x == 1);

        auto y = e::pointer_to(x);
        e::store(y, 2);
        REQUIRE (x == 2);
    }

    SECTION ("Mutable")
    {
        int x = 0;
        auto y = e::at(x);
        REQUIRE (e::at(y) == 0);

        e::at(y) = 1;
        REQUIRE (e::at(y) == 1);
    }
}

SCENARIO ("Using linear traversal functions", "[position]")
{
}
