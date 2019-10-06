#include "catch.hpp"

#include "lexicographical.h"

namespace e = elements;

SCENARIO ("Lexicographical comparison", "[lexicographical]")
{
    int x[]{0, 1, 2, 3, 4};
    int y[]{0, 1, -2, 3, 4};

    SECTION ("equivalent_lexicographical")
    {
        SECTION ("Checking if two ranges are lexicographically equivalent")
        {
            REQUIRE (e::equivalent_lexicographical(x, x + 5, x, x + 5, e::equal<int>{}));
            REQUIRE (!e::equivalent_lexicographical(x, x + 4, x, x + 5, e::equal<int>{}));
            REQUIRE (!e::equivalent_lexicographical(x, x + 5, x, x + 4, e::equal<int>{}));
            REQUIRE (!e::equivalent_lexicographical(x, x + 5, y, y + 5, e::equal<int>{}));
        }
    }

    SECTION ("equal_lexicographical")
    {
        SECTION ("Checking if two ranges are lexicographically equal")
        {
            REQUIRE (e::equal_lexicographical(x, x + 5, x, x + 5));
            REQUIRE (!e::equal_lexicographical(x, x + 4, x, x + 5));
            REQUIRE (!e::equal_lexicographical(x, x + 5, x, x + 4));
            REQUIRE (!e::equal_lexicographical(x, x + 5, y, y + 5));
        }
    }

    SECTION ("compare_lexicographical")
    {
        SECTION ("Checking if one range is lexicographically less than another")
        {
            REQUIRE (e::compare_lexicographical(x, x, x, x + 5, e::less<int>{}));
            REQUIRE (!e::compare_lexicographical(x, x + 5, x, x, e::less<int>{}));
            REQUIRE (e::compare_lexicographical(x, x + 4, x, x + 5, e::less<int>{}));
            REQUIRE (!e::compare_lexicographical(x, x + 5, x, x + 4, e::less<int>{}));
            REQUIRE (!e::compare_lexicographical(x, x + 5, x, x + 5, e::less<int>{}));
            REQUIRE (!e::compare_lexicographical(x, x + 5, y, y + 5, e::less<int>{}));
            REQUIRE (e::compare_lexicographical(y, y + 5, x, x + 5, e::less<int>{}));
        }
    }

    SECTION ("less_lexicographical")
    {
        SECTION ("Checking if one range is lexicographically less than another")
        {
            REQUIRE (e::less_lexicographical(x, x, x, x + 5));
            REQUIRE (!e::less_lexicographical(x, x + 5, x, x));
            REQUIRE (e::less_lexicographical(x, x + 4, x, x + 5));
            REQUIRE (!e::less_lexicographical(x, x + 5, x, x + 4));
            REQUIRE (!e::less_lexicographical(x, x + 5, x, x + 5));
            REQUIRE (!e::less_lexicographical(x, x + 5, y, y + 5));
            REQUIRE (e::less_lexicographical(y, y + 5, x, x + 5));
        }
    }
}
