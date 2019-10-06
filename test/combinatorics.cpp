#include "catch.hpp"

#include "combinatorics.h"

namespace e = elements;

SCENARIO ("Combinatorics", "[combinatorics]")
{
    SECTION ("Binomial coefficients")
    {
        REQUIRE (e::choose(0, 0) == 1);

        REQUIRE (e::choose(1, 0) == 1);
        REQUIRE (e::choose(1, 1) == 1);

        REQUIRE (e::choose(2, 0) == 1);
        REQUIRE (e::choose(2, 1) == 2);
        REQUIRE (e::choose(2, 2) == 1);

        REQUIRE (e::choose(3, 0) == 1);
        REQUIRE (e::choose(3, 1) == 3);
        REQUIRE (e::choose(3, 2) == 3);
        REQUIRE (e::choose(3, 3) == 1);

        REQUIRE (e::choose(4, 0) == 1);
        REQUIRE (e::choose(4, 1) == 4);
        REQUIRE (e::choose(4, 2) == 6);
        REQUIRE (e::choose(4, 3) == 4);
        REQUIRE (e::choose(4, 4) == 1);

        REQUIRE (e::choose(5, 0) == 1);
        REQUIRE (e::choose(5, 1) == 5);
        REQUIRE (e::choose(5, 2) == 10);
        REQUIRE (e::choose(5, 3) == 10);
        REQUIRE (e::choose(5, 4) == 5);
        REQUIRE (e::choose(5, 5) == 1);
    }
}
