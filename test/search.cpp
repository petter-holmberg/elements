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

    SECTION ("search_match")
    {
        SECTION ("Searcing for a match between two empty ranges")
        {
            auto pos = e::search_match(x, x, x, x);
            REQUIRE (e::get<0>(pos) == x);
            REQUIRE (e::get<1>(pos) == x);
        }

        SECTION ("Searcing for a match between two arrays of equal length where there is a match")
        {
            int y[]{4, 3, 2, 1, 0};

            auto pos = e::search_match(x, x + 5, y, y + 5);
            REQUIRE (e::get<0>(pos) == x + 2);
            REQUIRE (e::get<1>(pos) == y + 2);
        }

        SECTION ("Searcing for a match between two arrays of equal length where there is no match")
        {
            int y[]{1, 2, 3, 4, 5};

            auto pos = e::search_match(x, x + 5, y, y + 5);
            REQUIRE (e::get<0>(pos) == x + 5);
            REQUIRE (e::get<1>(pos) == y + 5);
        }

        SECTION ("Searcing for a match between two arrays of different length where there is a match")
        {
            int y[]{4, 3, 2, 1};

            auto pos = e::search_match(x, x + 5, y, y + 4);
            REQUIRE (e::get<0>(pos) == x + 2);
            REQUIRE (e::get<1>(pos) == y + 2);

            pos = e::search_match(y, y + 4, x, x + 5);
            REQUIRE (e::get<0>(pos) == y + 2);
            REQUIRE (e::get<1>(pos) == x + 2);
        }

        SECTION ("Searcing for a match between two arrays of different length where there is no match")
        {
            int y[]{1, 2, 3, 4};

            auto pos = e::search_match(x, x + 5, y, y + 4);
            REQUIRE (e::get<0>(pos) == x + 4);
            REQUIRE (e::get<1>(pos) == y + 4);

            pos = e::search_match(y, y + 4, x, x + 5);
            REQUIRE (e::get<0>(pos) == y + 4);
            REQUIRE (e::get<1>(pos) == x + 4);
        }
    }

    SECTION ("search_mismatch")
    {
        SECTION ("Searcing for a mismatch between two empty ranges")
        {
            auto pos = e::search_mismatch(x, x, x, x);
            REQUIRE (e::get<0>(pos) == x);
            REQUIRE (e::get<1>(pos) == x);
        }

        SECTION ("Searcing for a mismatch between two arrays of equal length where there is a mismatch")
        {
            int y[]{0, 1, 0, 3, 4};

            auto pos = e::search_mismatch(x, x + 5, y, y + 5);
            REQUIRE (e::get<0>(pos) == x + 2);
            REQUIRE (e::get<1>(pos) == y + 2);
        }

        SECTION ("Searcing for a mismatch between two arrays of equal length where there is no mismatch")
        {
            int y[]{0, 1, 2, 3, 4};

            auto pos = e::search_mismatch(x, x + 5, y, y + 5);
            REQUIRE (e::get<0>(pos) == x + 5);
            REQUIRE (e::get<1>(pos) == y + 5);
        }

        SECTION ("Searcing for a mismatch between two arrays of different length where there is a mismatch")
        {
            int y[]{0, 1, 0, 3};

            auto pos = e::search_mismatch(x, x + 5, y, y + 4);
            REQUIRE (e::get<0>(pos) == x + 2);
            REQUIRE (e::get<1>(pos) == y + 2);

            pos = e::search_mismatch(y, y + 4, x, x + 5);
            REQUIRE (e::get<0>(pos) == y + 2);
            REQUIRE (e::get<1>(pos) == x + 2);
        }

        SECTION ("Searcing for a mismatch between two arrays of different length where there is no mismatch")
        {
            int y[]{0, 1, 2, 3};

            auto pos = e::search_mismatch(x, x + 5, y, y + 4);
            REQUIRE (e::get<0>(pos) == x + 4);
            REQUIRE (e::get<1>(pos) == y + 4);

            pos = e::search_mismatch(y, y + 4, x, x + 5);
            REQUIRE (e::get<0>(pos) == y + 4);
            REQUIRE (e::get<1>(pos) == x + 4);
        }
    }
}