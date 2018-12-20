#include "catch.hpp"

#include "elements.h"

namespace e = elements;

SCENARIO ("Lexicographical comparison", "[lexicographical]")
{
    int x[]{0, 1, 2, 3, 4};
    int y[]{0, 1, -2, 3, 4};

    SECTION ("lexicographical_equivalent")
    {
        SECTION ("Checking if two ranges are lexicographically equivalent")
        {
            REQUIRE (e::lexicographical_equivalent(x, x + 5, x, x + 5, e::equal<int>{}));
            REQUIRE (!e::lexicographical_equivalent(x, x + 4, x, x + 5, e::equal<int>{}));
            REQUIRE (!e::lexicographical_equivalent(x, x + 5, x, x + 4, e::equal<int>{}));
            REQUIRE (!e::lexicographical_equivalent(x, x + 5, y, y + 5, e::equal<int>{}));
        }
    }

    SECTION ("lexicographical_equal")
    {
        SECTION ("Checking if two ranges are lexicographically equal")
        {
            REQUIRE (e::lexicographical_equal(x, x + 5, x, x + 5));
            REQUIRE (!e::lexicographical_equal(x, x + 4, x, x + 5));
            REQUIRE (!e::lexicographical_equal(x, x + 5, x, x + 4));
            REQUIRE (!e::lexicographical_equal(x, x + 5, y, y + 5));
        }
    }

    SECTION ("lexicographical_compare")
    {
        SECTION ("Checking if one range is lexicographically less than another")
        {
            REQUIRE (e::lexicographical_compare(x, x, x, x + 5, e::less<int>{}));
            REQUIRE (!e::lexicographical_compare(x, x + 5, x, x, e::less<int>{}));
            REQUIRE (e::lexicographical_compare(x, x + 4, x, x + 5, e::less<int>{}));
            REQUIRE (!e::lexicographical_compare(x, x + 5, x, x + 4, e::less<int>{}));
            REQUIRE (!e::lexicographical_compare(x, x + 5, x, x + 5, e::less<int>{}));
            REQUIRE (!e::lexicographical_compare(x, x + 5, y, y + 5, e::less<int>{}));
            REQUIRE (e::lexicographical_compare(y, y + 5, x, x + 5, e::less<int>{}));
        }
    }

    SECTION ("lexicographical_less")
    {
        SECTION ("Checking if one range is lexicographically less than another")
        {
            REQUIRE (e::lexicographical_less(x, x, x, x + 5));
            REQUIRE (!e::lexicographical_less(x, x + 5, x, x));
            REQUIRE (e::lexicographical_less(x, x + 4, x, x + 5));
            REQUIRE (!e::lexicographical_less(x, x + 5, x, x + 4));
            REQUIRE (!e::lexicographical_less(x, x + 5, x, x + 5));
            REQUIRE (!e::lexicographical_less(x, x + 5, y, y + 5));
            REQUIRE (e::lexicographical_less(y, y + 5, x, x + 5));
        }
    }
}
