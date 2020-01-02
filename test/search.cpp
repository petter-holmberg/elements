#include "catch.hpp"

#include "search.h"

namespace e = elements;

SCENARIO ("Linear search", "[search]")
{
    int x[]{0, 1, 2, 3, 4};
    auto is_even = [](int i){ return i % 2 == 0; };

    SECTION ("search")
    {
        SECTION ("Searcing for an existing element")
        {
            auto pos = e::search(e::loadable_position(x), x + 5, 2);
            REQUIRE (!e::precedes(pos, x + 2));
        }

        SECTION ("Searching for a non-existing element")
        {
            auto pos = e::search(x, x + 5, 5);
            REQUIRE (pos == x + 5);
        }

        SECTION ("Searching for the absence of an existing element")
        {
            auto pos = e::search_not(e::loadable_position(x), x + 5, 0);
            REQUIRE (!e::precedes(pos, x + 1));
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
            auto pos = e::search_if(e::loadable_position(x), x + 5, is_even);
            REQUIRE (!e::precedes(pos, +x));
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
            auto pos = e::search_unguarded(e::loadable_position(x), 2);
            REQUIRE (!e::precedes(pos, x + 2));
        }

        SECTION ("Searching for the absence of an existing element")
        {
            auto pos = e::search_not_unguarded(e::loadable_position(x), 0);
            REQUIRE (!e::precedes(pos, x + 1));
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
            auto pos = e::search_if_unguarded(e::loadable_position(x), is_even);
            REQUIRE (!e::precedes(pos, +x));
        }

        SECTION ("Searcing for an odd element")
        {
            auto pos = e::search_if_not_unguarded(e::loadable_position(x), is_even);
            REQUIRE (!e::precedes(pos, x + 1));
        }
    }

    SECTION ("search_guarded")
    {
        SECTION ("Searching for an existing element before the last element")
        {
            auto pos = e::search_guarded(x, x + 4, 2);
            REQUIRE (pos == x + 2);
        }

        SECTION ("Searching for the last element")
        {
            auto pos = e::search_guarded(x, x + 4, 4);
            REQUIRE (pos == x + 4);
        }

        SECTION ("Searching for a non-existing element")
        {
            auto pos = e::search_guarded(x, x + 4, 5);
            REQUIRE (pos == x + 5);
        }

        SECTION ("Searching for the absence of an existing element before the last element")
        {
            auto pos = e::search_not_guarded(x, x + 4, 0, 1);
            REQUIRE (pos == x + 1);
        }

        SECTION ("Searching for the absence of the last element")
        {
            int y[]{0, 0, 0, 0, 1};
            auto pos = e::search_not_guarded(y, y + 4, 0, 1);
            REQUIRE (pos == y + 4);
        }

        SECTION ("Searching for the absence of the only element")
        {
            int y[]{0, 0, 0, 0, 0};
            auto pos = e::search_not_guarded(y, y + 4, 0, 1);
            REQUIRE (pos == y + 5);
        }
    }

    SECTION ("search_if_guarded")
    {
        SECTION ("Searching for an even element that exists before the last element")
        {
            auto pos = e::search_if_guarded(x, x + 4, is_even, 0);
            REQUIRE (pos == +x);
        }

        SECTION ("Searching for an even element that is the last element")
        {
            int y[]{1, 3, 5, 7, 2};
            auto pos = e::search_if_guarded(y, y + 4, is_even, 0);
            REQUIRE (pos == y + 4);
        }

        SECTION ("Searching for an even element when none exists")
        {
            int y[]{1, 3, 5, 7, 9};
            auto pos = e::search_if_guarded(y, y + 4, is_even, 0);
            REQUIRE (pos == y + 5);
        }

        SECTION ("Searching for the absence of an even element that exists before the last element")
        {
            auto pos = e::search_if_not_guarded(x, x + 4, is_even, 1);
            REQUIRE (pos == x + 1);
        }

        SECTION ("Searching for the absence of an even element that is the last element")
        {
            int y[]{0, 2, 4, 6, 1};
            auto pos = e::search_if_not_guarded(y, y + 4, is_even, 1);
            REQUIRE (pos == y + 4);
        }

        SECTION ("Searching for the absence of an even element when all are even")
        {
            int y[]{0, 2, 4, 6, 8};
            auto pos = e::search_if_not_guarded(y, y + 4, is_even, 1);
            REQUIRE (pos == y + 5);
        }
    }

    SECTION ("search_match")
    {
        SECTION ("Searcing for a match between two empty ranges")
        {
            auto pos = e::search_match(e::loadable_position(x), x, x, x);
            REQUIRE (!e::precedes(e::get<0>(pos), +x));
            REQUIRE (!e::precedes(e::get<1>(pos), +x));
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
            auto pos = e::search_mismatch(e::loadable_position(x), x, x, x);
            REQUIRE (!e::precedes(e::get<0>(pos), +x));
            REQUIRE (!e::precedes(e::get<1>(pos), +x));
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

    SECTION ("search_adjacent_match")
    {
        SECTION ("Positions")
        {
            SECTION ("Searcing for an adjacent equality match in an array that contains it")
            {
                int y[]{0, 1, 2, 3, 3, 4, 5};

                auto pos = e::search_adjacent_match(e::position(y), y + 7);
                REQUIRE (!e::precedes(pos, y + 4));
            }

            SECTION ("Searcing for an adjacent equality match in an array that doesn't contain it")
            {
                int y[]{0, 1, 2, 3, 4, 5, 6};

                auto pos = e::search_adjacent_match(e::position(y), y + 7);
                REQUIRE (!e::precedes(pos, y + 7));
            }

            SECTION ("Searcing for an adjacent equality mismatch in an array that contains it")
            {
                int y[]{0, 0, 0, 0, 1, 1, 1};

                auto pos = e::search_adjacent_mismatch(e::position(y), y + 7);
                REQUIRE (!e::precedes(pos, y + 4));
            }

            SECTION ("Searcing for an adjacent equality mismatch in an array that doesn't contain it")
            {
                int y[]{0, 0, 0, 0, 0, 0, 0};

                auto pos = e::search_adjacent_mismatch(e::position(y), y + 7);
                REQUIRE (!e::precedes(pos, y + 7));
            }
        }

        SECTION ("Forward positions")
        {
            SECTION ("Searcing for an adjacent equality match in an array that contains it")
            {
                int y[]{0, 1, 2, 3, 3, 4, 5};

                auto pos = e::search_adjacent_match(e::forward_position(y), y + 7);
                REQUIRE (!e::precedes(pos, y + 4));
            }

            SECTION ("Searcing for an adjacent equality match in an array that doesn't contain it")
            {
                int y[]{0, 1, 2, 3, 4, 5, 6};

                auto pos = e::search_adjacent_match(e::forward_position(y), y + 7);
                REQUIRE (!e::precedes(pos, y + 7));
            }

            SECTION ("Searcing for an adjacent equality mismatch in an array that contains it")
            {
                int y[]{0, 0, 0, 0, 1, 1, 1};

                auto pos = e::search_adjacent_mismatch(e::forward_position(y), y + 7);
                REQUIRE (!e::precedes(pos, y + 4));
            }

            SECTION ("Searcing for an adjacent equality mismatch in an array that doesn't contain it")
            {
                int y[]{0, 0, 0, 0, 0, 0, 0};

                auto pos = e::search_adjacent_mismatch(e::forward_position(y), y + 7);
                REQUIRE (!e::precedes(pos, y + 7));
            }
        }
    }
}

SCENARIO ("Sub-sequence search (Knuth-Morris-Pratt)", "[search]")
{
    SECTION ("Searching with two arrays of integers, the second being a short subsequence of the first")
    {
        int x[]{0, 1, 2, 3, 4};
        int y[]{2, 3};
        ptrdiff_t buffer[2];

        auto pos = e::search_subsequence(x, x + 5, y, y + 2, buffer);
        REQUIRE (pos == x + 2);
    }

    SECTION ("Searching with two arrays of integers, the second being a long subsequence of the first")
    {
        int x[]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
        int y[]{0, 0, 0, 0, 0, 0, 0, 1};
        ptrdiff_t buffer[8];

        auto pos = e::search_subsequence(x, x + 15, y, y + 8, buffer);

        REQUIRE (pos == x + 7);
    }

    SECTION ("Searching with two arrays of integers, the second not being a subsequence of the first")
    {
        int x[]{0, 1, 2, 3, 4};
        int y[]{3, 2};
        ptrdiff_t buffer[2];

        auto pos = e::search_subsequence(x, x + 5, y, y + 2, buffer);

        REQUIRE (pos == x + 5);
    }
}
