#include "catch.hpp"

#include "elements.h"

namespace e = elements;

SCENARIO ("Linear search", "[search]")
{
    int x[]{0, 1, 2, 3, 4};
    auto is_even = [](int i){ return i % 2 == 0; };

    SECTION ("search")
    {
        SECTION ("Searcing for an existing element")
        {
            auto pos = e::search(x, x + 5, 2);
            REQUIRE (pos == x + 2);
        }

        SECTION ("Searching for a non-existing element")
        {
            auto pos = e::search(x, x + 5, 5);
            REQUIRE (pos == x + 5);
        }

        SECTION ("Searching for the absence of an existing element")
        {
            auto pos = e::search_not(x, x + 5, 0);
            REQUIRE (pos == x + 1);
        }

        SECTION ("Searching for the absence of a non-existing element")
        {
            auto pos = e::search_not(x, x + 5, 5);
            REQUIRE (pos == x);
        }
    }

    SECTION ("search_if")
    {
        SECTION ("Searcing for an even element")
        {
            auto pos = e::search_if(x, x + 5, is_even);
            REQUIRE (pos == x);
        }

        SECTION ("Searcing for an odd element")
        {
            auto pos = e::search_if_not(x, x + 5, is_even);
            REQUIRE (pos == x + 1);
        }
    }

    SECTION ("search_unguarded")
    {
        SECTION ("Searcing for an existing element")
        {
            auto pos = e::search_unguarded(x, 2);
            REQUIRE (pos == x + 2);
        }

        SECTION ("Searching for the absence of an existing element")
        {
            auto pos = e::search_not_unguarded(x, 0);
            REQUIRE (pos == x + 1);
        }

        SECTION ("Searching for the absence of a non-existing element")
        {
            auto pos = e::search_not_unguarded(x, 5);
            REQUIRE (pos == x);
        }
    }

    SECTION ("search_if_unguarded")
    {
        SECTION ("Searcing for an even element")
        {
            auto pos = e::search_if_unguarded(x, is_even);
            REQUIRE (pos == x);
        }

        SECTION ("Searcing for an odd element")
        {
            auto pos = e::search_if_not_unguarded(x, is_even);
            REQUIRE (pos == x + 1);
        }
    }
}
