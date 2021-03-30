#include "catch.hpp"

#include "search.h"

namespace e = elements;

SCENARIO ("Linear search", "[search]")
{
    int x[]{0, 1, 2, 3, 4};
    auto is_even = [](int i){ return i % 2 == 0; };

    SECTION ("search, search_not")
    {
        SECTION ("Searcing for an existing element")
        {
            auto cur = e::search(e::loadable_cursor(x), x + 5, 2);
            REQUIRE (!e::precedes(cur, x + 2));
        }

        SECTION ("Searching for a non-existing element")
        {
            auto cur = e::search(x, x + 5, 5);
            REQUIRE (cur == x + 5);
        }

        SECTION ("Searching for the absence of an existing element")
        {
            auto cur = e::search_not(e::loadable_cursor(x), x + 5, 0);
            REQUIRE (!e::precedes(cur, x + 1));
        }

        SECTION ("Searching for the absence of a non-existing element")
        {
            auto cur = e::search_not(x, x + 5, 5);
            REQUIRE (cur == x);
        }
    }

    SECTION ("search_n, search_not_n")
    {
        SECTION ("Searcing for an existing element")
        {
            auto range = e::search_n(e::loadable_cursor(x), 5, 2);
            REQUIRE (!e::precedes(e::get<0>(range), x + 2));
        }

        SECTION ("Searching for a non-existing element")
        {
            auto range = e::search_n(x, 5, 5);
            REQUIRE (e::get<0>(range) == x + 5);
        }

        SECTION ("Searching for the absence of an existing element")
        {
            auto range = e::search_not_n(e::loadable_cursor(x), 5, 0);
            REQUIRE (!e::precedes(e::get<0>(range), x + 1));
        }

        SECTION ("Searching for the absence of a non-existing element")
        {
            auto range = e::search_not_n(x, 5, 5);
            REQUIRE (e::get<0>(range) == x);
        }
    }

    SECTION ("search_backward, search_backward_not")
    {
        SECTION ("Searcing backward for an existing element")
        {
            auto cur = e::search_backward(x, x + 5, 2);
            REQUIRE (!e::precedes(cur, x + 3));
        }

        SECTION ("Searching backward for a non-existing element")
        {
            auto cur = e::search_backward(x, x + 5, 5);
            REQUIRE (cur == x);
        }

        SECTION ("Searching backward for the absence of an existing element")
        {
            auto cur = e::search_backward_not(x, x + 5, 0);
            REQUIRE (!e::precedes(cur, x + 5));
        }

        SECTION ("Searching backward for the absence of a non-existing element")
        {
            auto cur = e::search_backward_not(x, x + 5, 5);
            REQUIRE (cur == x + 5);
        }
    }

    SECTION ("search_backward_n, search_backward_not_n")
    {
        SECTION ("Searcing backward for an existing element")
        {
            auto range = e::search_backward_n(x, 5, 2);
            REQUIRE (!e::precedes(e::get<0>(range), x + 3));
        }

        SECTION ("Searching backward for a non-existing element")
        {
            auto range = e::search_backward_n(x, 5, 5);
            REQUIRE (e::get<0>(range) == x);
        }

        SECTION ("Searching backward for the absence of an existing element")
        {
            auto range = e::search_backward_not_n(x, 5, 0);
            REQUIRE (!e::precedes(e::get<0>(range), x + 5));
        }

        SECTION ("Searching backward for the absence of a non-existing element")
        {
            auto range = e::search_backward_not_n(x, 5, 5);
            REQUIRE (e::get<0>(range) == x + 5);
        }
    }

    SECTION ("search_if, search_if_not")
    {
        SECTION ("Searcing for an even element")
        {
            auto cur = e::search_if(e::loadable_cursor(x), x + 5, is_even);
            REQUIRE (!e::precedes(cur, +x));
        }

        SECTION ("Searcing for an odd element")
        {
            auto cur = e::search_if_not(x, x + 5, is_even);
            REQUIRE (cur == x + 1);
        }
    }

    SECTION ("search_if_n, search_if_not_n")
    {
        SECTION ("Searcing for an even element")
        {
            auto range = e::search_if_n(e::loadable_cursor(x), 5, is_even);
            REQUIRE (!e::precedes(e::get<0>(range), +x));
        }

        SECTION ("Searcing for an odd element")
        {
            auto range = e::search_if_not_n(x, 5, is_even);
            REQUIRE (e::get<0>(range) == x + 1);
        }
    }

    SECTION ("search_backward_if, search_backward_if_not")
    {
        SECTION ("Searcing for an even element")
        {
            auto cur = e::search_backward_if(x, x + 5, is_even);
            REQUIRE (!e::precedes(cur, x + 5));
        }

        SECTION ("Searcing for an odd element")
        {
            auto cur = e::search_backward_if_not(x, x + 5, is_even);
            REQUIRE (cur == x + 4);
        }
    }

    SECTION ("search_backward_if_n, search_backward_if_not_n")
    {
        SECTION ("Searcing for an even element")
        {
            auto range = e::search_backward_if_n(x, 5, is_even);
            REQUIRE (!e::precedes(e::get<0>(range), x + 5));
        }

        SECTION ("Searcing for an odd element")
        {
            auto range = e::search_backward_if_not_n(x, 5, is_even);
            REQUIRE (e::get<0>(range) == x + 4);
        }
    }

    SECTION ("search_unguarded")
    {
        SECTION ("Searcing for an existing element")
        {
            auto cur = e::search_unguarded(e::loadable_cursor(x), 2);
            REQUIRE (!e::precedes(cur, x + 2));
        }

        SECTION ("Searching for the absence of an existing element")
        {
            auto cur = e::search_not_unguarded(e::loadable_cursor(x), 0);
            REQUIRE (!e::precedes(cur, x + 1));
        }

        SECTION ("Searching for the absence of a non-existing element")
        {
            auto cur = e::search_not_unguarded(x, 5);
            REQUIRE (cur == x);
        }
    }

    SECTION ("search_if_unguarded")
    {
        SECTION ("Searcing for an even element")
        {
            auto cur = e::search_if_unguarded(e::loadable_cursor(x), is_even);
            REQUIRE (!e::precedes(cur, +x));
        }

        SECTION ("Searcing for an odd element")
        {
            auto cur = e::search_if_not_unguarded(e::loadable_cursor(x), is_even);
            REQUIRE (!e::precedes(cur, x + 1));
        }
    }

    SECTION ("search_guarded")
    {
        SECTION ("Searching for an existing element before the last element")
        {
            auto cur = e::search_guarded(x, x + 4, 2);
            REQUIRE (cur == x + 2);
        }

        SECTION ("Searching for the last element")
        {
            auto cur = e::search_guarded(x, x + 4, 4);
            REQUIRE (cur == x + 4);
        }

        SECTION ("Searching for a non-existing element")
        {
            auto cur = e::search_guarded(x, x + 4, 5);
            REQUIRE (cur == x + 5);
        }

        SECTION ("Searching for the absence of an existing element before the last element")
        {
            auto cur = e::search_not_guarded(x, x + 4, 0, 1);
            REQUIRE (cur == x + 1);
        }

        SECTION ("Searching for the absence of the last element")
        {
            int y[]{0, 0, 0, 0, 1};
            auto cur = e::search_not_guarded(y, y + 4, 0, 1);
            REQUIRE (cur == y + 4);
        }

        SECTION ("Searching for the absence of the only element")
        {
            int y[]{0, 0, 0, 0, 0};
            auto cur = e::search_not_guarded(y, y + 4, 0, 1);
            REQUIRE (cur == y + 5);
        }
    }

    SECTION ("search_if_guarded")
    {
        SECTION ("Searching for an even element that exists before the last element")
        {
            auto cur = e::search_if_guarded(x, x + 4, is_even, 0);
            REQUIRE (cur == +x);
        }

        SECTION ("Searching for an even element that is the last element")
        {
            int y[]{1, 3, 5, 7, 2};
            auto cur = e::search_if_guarded(y, y + 4, is_even, 0);
            REQUIRE (cur == y + 4);
        }

        SECTION ("Searching for an even element when none exists")
        {
            int y[]{1, 3, 5, 7, 9};
            auto cur = e::search_if_guarded(y, y + 4, is_even, 0);
            REQUIRE (cur == y + 5);
        }

        SECTION ("Searching for the absence of an even element that exists before the last element")
        {
            auto cur = e::search_if_not_guarded(x, x + 4, is_even, 1);
            REQUIRE (cur == x + 1);
        }

        SECTION ("Searching for the absence of an even element that is the last element")
        {
            int y[]{0, 2, 4, 6, 1};
            auto cur = e::search_if_not_guarded(y, y + 4, is_even, 1);
            REQUIRE (cur == y + 4);
        }

        SECTION ("Searching for the absence of an even element when all are even")
        {
            int y[]{0, 2, 4, 6, 8};
            auto cur = e::search_if_not_guarded(y, y + 4, is_even, 1);
            REQUIRE (cur == y + 5);
        }
    }

    SECTION ("search_match")
    {
        SECTION ("Searcing for a match between two empty ranges")
        {
            auto cur = e::search_match(e::loadable_cursor(x), x, x, x);
            REQUIRE (!e::precedes(e::get<0>(cur), +x));
            REQUIRE (!e::precedes(e::get<1>(cur), +x));
        }

        SECTION ("Searcing for a match between two arrays of equal length where there is a match")
        {
            int y[]{4, 3, 2, 1, 0};

            auto cur = e::search_match(x, x + 5, y, y + 5);
            REQUIRE (e::get<0>(cur) == x + 2);
            REQUIRE (e::get<1>(cur) == y + 2);
        }

        SECTION ("Searcing for a match between two arrays of equal length where there is no match")
        {
            int y[]{1, 2, 3, 4, 5};

            auto cur = e::search_match(x, x + 5, y, y + 5);
            REQUIRE (e::get<0>(cur) == x + 5);
            REQUIRE (e::get<1>(cur) == y + 5);
        }

        SECTION ("Searcing for a match between two arrays of different length where there is a match")
        {
            int y[]{4, 3, 2, 1};

            auto cur = e::search_match(x, x + 5, y, y + 4);
            REQUIRE (e::get<0>(cur) == x + 2);
            REQUIRE (e::get<1>(cur) == y + 2);

            cur = e::search_match(y, y + 4, x, x + 5);
            REQUIRE (e::get<0>(cur) == y + 2);
            REQUIRE (e::get<1>(cur) == x + 2);
        }

        SECTION ("Searcing for a match between two arrays of different length where there is no match")
        {
            int y[]{1, 2, 3, 4};

            auto cur = e::search_match(x, x + 5, y, y + 4);
            REQUIRE (e::get<0>(cur) == x + 4);
            REQUIRE (e::get<1>(cur) == y + 4);

            cur = e::search_match(y, y + 4, x, x + 5);
            REQUIRE (e::get<0>(cur) == y + 4);
            REQUIRE (e::get<1>(cur) == x + 4);
        }
    }

    SECTION ("search_match_n")
    {
        SECTION ("Searcing for a match between two empty ranges")
        {
            auto range = e::search_match_n(e::loadable_cursor(x), 0, x);
            REQUIRE (!e::precedes(e::get<0>(e::get<0>(range)), +x));
            REQUIRE (!e::precedes(e::get<1>(e::get<0>(range)), +x));
        }

        SECTION ("Searcing for a match between two arrays where there is a match")
        {
            int y[]{4, 3, 2, 1, 0};

            auto range = e::search_match_n(x, 5, y);
            REQUIRE (e::get<0>(e::get<0>(range)) == x + 2);
            REQUIRE (e::get<1>(e::get<0>(range)) == y + 2);
        }

        SECTION ("Searcing for a match between two arrays where there is no match")
        {
            int y[]{1, 2, 3, 4, 5};

            auto range = e::search_match_n(x, 5, y);
            REQUIRE (e::get<0>(e::get<0>(range)) == x + 5);
            REQUIRE (e::get<1>(e::get<0>(range)) == y + 5);
        }
    }

    SECTION ("search_mismatch")
    {
        SECTION ("Searcing for a mismatch between two empty ranges")
        {
            auto cur = e::search_mismatch(e::loadable_cursor(x), x, x, x);
            REQUIRE (!e::precedes(e::get<0>(cur), +x));
            REQUIRE (!e::precedes(e::get<1>(cur), +x));
        }

        SECTION ("Searcing for a mismatch between two arrays of equal length where there is a mismatch")
        {
            int y[]{0, 1, 0, 3, 4};

            auto cur = e::search_mismatch(x, x + 5, y, y + 5);
            REQUIRE (e::get<0>(cur) == x + 2);
            REQUIRE (e::get<1>(cur) == y + 2);
        }

        SECTION ("Searcing for a mismatch between two arrays of equal length where there is no mismatch")
        {
            int y[]{0, 1, 2, 3, 4};

            auto cur = e::search_mismatch(x, x + 5, y, y + 5);
            REQUIRE (e::get<0>(cur) == x + 5);
            REQUIRE (e::get<1>(cur) == y + 5);
        }

        SECTION ("Searcing for a mismatch between two arrays of different length where there is a mismatch")
        {
            int y[]{0, 1, 0, 3};

            auto cur = e::search_mismatch(x, x + 5, y, y + 4);
            REQUIRE (e::get<0>(cur) == x + 2);
            REQUIRE (e::get<1>(cur) == y + 2);

            cur = e::search_mismatch(y, y + 4, x, x + 5);
            REQUIRE (e::get<0>(cur) == y + 2);
            REQUIRE (e::get<1>(cur) == x + 2);
        }

        SECTION ("Searcing for a mismatch between two arrays of different length where there is no mismatch")
        {
            int y[]{0, 1, 2, 3};

            auto cur = e::search_mismatch(x, x + 5, y, y + 4);
            REQUIRE (e::get<0>(cur) == x + 4);
            REQUIRE (e::get<1>(cur) == y + 4);

            cur = e::search_mismatch(y, y + 4, x, x + 5);
            REQUIRE (e::get<0>(cur) == y + 4);
            REQUIRE (e::get<1>(cur) == x + 4);
        }
    }

    SECTION ("search_mismatch_n")
    {
        SECTION ("Searcing for a mismatch between two empty ranges")
        {
            auto range = e::search_mismatch_n(e::loadable_cursor(x), 0, x);
            REQUIRE (!e::precedes(e::get<0>(e::get<0>(range)), +x));
            REQUIRE (!e::precedes(e::get<1>(e::get<0>(range)), +x));
        }

        SECTION ("Searcing for a mismatch between two arrays where there is a mismatch")
        {
            int y[]{0, 1, 0, 3, 4};

            auto range = e::search_mismatch_n(x, 5, y);
            REQUIRE (e::get<0>(e::get<0>(range)) == x + 2);
            REQUIRE (e::get<1>(e::get<0>(range)) == y + 2);
        }

        SECTION ("Searcing for a mismatch between two arrays where there is no mismatch")
        {
            int y[]{0, 1, 2, 3, 4};

            auto range = e::search_mismatch_n(x, 5, y);
            REQUIRE (e::get<0>(e::get<0>(range)) == x + 5);
            REQUIRE (e::get<1>(e::get<0>(range)) == y + 5);
        }
    }

    SECTION ("search_adjacent_match, search_adjacent_mismatch")
    {
        SECTION ("Cursors")
        {
            SECTION ("Searcing for an adjacent equality match in an array that contains it")
            {
                int y[]{0, 1, 2, 3, 3, 4, 5};

                auto cur = e::search_adjacent_match(e::cursor(y), y + 7);
                REQUIRE (!e::precedes(cur, y + 4));
            }

            SECTION ("Searcing for an adjacent equality match in an array that doesn't contain it")
            {
                int y[]{0, 1, 2, 3, 4, 5, 6};

                auto cur = e::search_adjacent_match(e::cursor(y), y + 7);
                REQUIRE (!e::precedes(cur, y + 7));
            }

            SECTION ("Searcing for an adjacent equality mismatch in an array that contains it")
            {
                int y[]{0, 0, 0, 0, 1, 1, 1};

                auto cur = e::search_adjacent_mismatch(e::cursor(y), y + 7);
                REQUIRE (!e::precedes(cur, y + 4));
            }

            SECTION ("Searcing for an adjacent equality mismatch in an array that doesn't contain it")
            {
                int y[]{0, 0, 0, 0, 0, 0, 0};

                auto cur = e::search_adjacent_mismatch(e::cursor(y), y + 7);
                REQUIRE (!e::precedes(cur, y + 7));
            }
        }

        SECTION ("Forward cursors")
        {
            SECTION ("Searcing for an adjacent equality match in an array that contains it")
            {
                int y[]{0, 1, 2, 3, 3, 4, 5};

                auto cur = e::search_adjacent_match(e::forward_cursor(y), y + 7);
                REQUIRE (!e::precedes(cur, y + 4));
            }

            SECTION ("Searcing for an adjacent equality match in an array that doesn't contain it")
            {
                int y[]{0, 1, 2, 3, 4, 5, 6};

                auto cur = e::search_adjacent_match(e::forward_cursor(y), y + 7);
                REQUIRE (!e::precedes(cur, y + 7));
            }

            SECTION ("Searcing for an adjacent equality mismatch in an array that contains it")
            {
                int y[]{0, 0, 0, 0, 1, 1, 1};

                auto cur = e::search_adjacent_mismatch(e::forward_cursor(y), y + 7);
                REQUIRE (!e::precedes(cur, y + 4));
            }

            SECTION ("Searcing for an adjacent equality mismatch in an array that doesn't contain it")
            {
                int y[]{0, 0, 0, 0, 0, 0, 0};

                auto cur = e::search_adjacent_mismatch(e::forward_cursor(y), y + 7);
                REQUIRE (!e::precedes(cur, y + 7));
            }
        }
    }

    SECTION ("search_adjacent_match_n, search_adjacent_mismatch_n")
    {
        SECTION ("Cursors")
        {
            SECTION ("Searcing for an adjacent equality match in an array that contains it")
            {
                int y[]{0, 1, 2, 3, 3, 4, 5};

                auto range = e::search_adjacent_match_n(e::cursor(y), 7);
                REQUIRE (!e::precedes(e::get<0>(range), y + 4));
            }

            SECTION ("Searcing for an adjacent equality match in an array that doesn't contain it")
            {
                int y[]{0, 1, 2, 3, 4, 5, 6};

                auto range = e::search_adjacent_match_n(e::cursor(y), 7);
                REQUIRE (!e::precedes(e::get<0>(range), y + 7));
            }

            SECTION ("Searcing for an adjacent equality mismatch in an array that contains it")
            {
                int y[]{0, 0, 0, 0, 1, 1, 1};

                auto range = e::search_adjacent_mismatch_n(e::cursor(y), 7);
                REQUIRE (!e::precedes(e::get<0>(range), y + 4));
            }

            SECTION ("Searcing for an adjacent equality mismatch in an array that doesn't contain it")
            {
                int y[]{0, 0, 0, 0, 0, 0, 0};

                auto range = e::search_adjacent_mismatch_n(e::cursor(y), 7);
                REQUIRE (!e::precedes(e::get<0>(range), y + 7));
            }
        }

        SECTION ("Forward cursors")
        {
            SECTION ("Searcing for an adjacent equality match in an array that contains it")
            {
                int y[]{0, 1, 2, 3, 3, 4, 5};

                auto range = e::search_adjacent_match_n(e::forward_cursor(y), 7);
                REQUIRE (!e::precedes(e::get<0>(range), y + 4));
            }

            SECTION ("Searcing for an adjacent equality match in an array that doesn't contain it")
            {
                int y[]{0, 1, 2, 3, 4, 5, 6};

                auto range = e::search_adjacent_match_n(e::forward_cursor(y), 7);
                REQUIRE (!e::precedes(e::get<0>(range), y + 7));
            }

            SECTION ("Searcing for an adjacent equality mismatch in an array that contains it")
            {
                int y[]{0, 0, 0, 0, 1, 1, 1};

                auto range = e::search_adjacent_mismatch_n(e::forward_cursor(y), 7);
                REQUIRE (!e::precedes(e::get<0>(range), y + 4));
            }

            SECTION ("Searcing for an adjacent equality mismatch in an array that doesn't contain it")
            {
                int y[]{0, 0, 0, 0, 0, 0, 0};

                auto range = e::search_adjacent_mismatch_n(e::forward_cursor(y), 7);
                REQUIRE (!e::precedes(e::get<0>(range), y + 7));
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

        auto cur = e::search_subsequence(x, x + 5, y, y + 2, buffer);
        REQUIRE (cur == x + 2);
    }

    SECTION ("Searching with two arrays of integers, the second being a long subsequence of the first")
    {
        int x[]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
        int y[]{0, 0, 0, 0, 0, 0, 0, 1};
        ptrdiff_t buffer[8];

        auto cur = e::search_subsequence(x, x + 15, y, y + 8, buffer);

        REQUIRE (cur == x + 7);
    }

    SECTION ("Searching with two arrays of integers, the second not being a subsequence of the first")
    {
        int x[]{0, 1, 2, 3, 4};
        int y[]{3, 2};
        ptrdiff_t buffer[2];

        auto cur = e::search_subsequence(x, x + 5, y, y + 2, buffer);

        REQUIRE (cur == x + 5);
    }
}
