#include "catch.hpp"

#include "search_binary.h"

namespace e = elements;

SCENARIO ("Binary search", "[search_binary]")
{
    SECTION ("Emtpy array of integers")
    {
        e::array<int> x{};

        auto pos = e::search_binary_lower(e::first(x), e::limit(x), 3);
        REQUIRE (pos == e::limit(x));

        pos = e::search_binary_upper(e::first(x), e::limit(x), 3);
        REQUIRE (pos == e::limit(x));

        auto range = e::search_binary(e::first(x), e::limit(x), 3);
        REQUIRE (e::first(range) == e::limit(x));
        REQUIRE (e::limit(range) == e::limit(x));
    }

    SECTION ("Array with one 3")
    {
        e::array<int> x{3};

        auto pos = e::search_binary_lower(e::first(x), e::limit(x), 3);
        REQUIRE (pos == e::first(x));

        pos = e::search_binary_upper(e::first(x), e::limit(x), 3);
        REQUIRE (pos == e::limit(x));

        auto range = e::search_binary(e::first(x), e::limit(x), 3);
        REQUIRE (e::first(range) == e::first(x));
        REQUIRE (e::limit(range) == e::limit(x));
    }

    SECTION ("Array with two 3s")
    {
        e::array<int> x{3, 3};

        auto pos = e::search_binary_lower(e::first(x), e::limit(x), 3);
        REQUIRE (pos == e::first(x));

        pos = e::search_binary_upper(e::first(x), e::limit(x), 3);
        REQUIRE (pos == e::limit(x));

        auto range = e::search_binary(e::first(x), e::limit(x), 3);
        REQUIRE (e::first(range) == e::first(x));
        REQUIRE (e::limit(range) == e::limit(x));
    }

    SECTION ("Array with two 3s at the beginning")
    {
        e::array<int> x{3, 3, 4, 5};

        auto pos = e::search_binary_lower(e::first(x), e::limit(x), 3);
        REQUIRE (pos == e::first(x));

        pos = e::search_binary_upper(e::first(x), e::limit(x), 3);
        REQUIRE (pos == e::first(x) + 2);

        auto range = e::search_binary(e::first(x), e::limit(x), 3);
        REQUIRE (e::first(range) == e::first(x));
        REQUIRE (e::limit(range) == e::first(x) + 2);
    }

    SECTION ("Array with two 3s at the end")
    {
        e::array<int> x{0, 1, 2, 3, 3};

        auto pos = e::search_binary_lower(e::first(x), e::limit(x), 3);
        REQUIRE (pos == e::first(x) + 3);

        pos = e::search_binary_upper(e::first(x), e::limit(x), 3);
        REQUIRE (pos == e::limit(x));

        auto range = e::search_binary(e::first(x), e::limit(x), 3);
        REQUIRE (e::first(range) == e::first(x) + 3);
        REQUIRE (e::limit(range) == e::limit(x));
    }

    SECTION ("Array with three 3s in the first half")
    {
        e::array<int> x{0, 3, 3, 3, 4, 5, 6, 7, 8};

        auto pos = e::search_binary_lower(e::first(x), e::limit(x), 3);
        REQUIRE (pos == e::first(x) + 1);

        pos = e::search_binary_upper(e::first(x), e::limit(x), 3);
        REQUIRE (pos == e::first(x) + 4);

        auto range = e::search_binary(e::first(x), e::limit(x), 3);
        REQUIRE (e::first(range) == e::first(x) + 1);
        REQUIRE (e::limit(range) == e::first(x) + 4);
    }

    SECTION ("Array with three 3s in the middle")
    {
        e::array<int> x{0, 3, 3, 3, 4};

        auto pos = e::search_binary_lower(e::first(x), e::limit(x), 3);
        REQUIRE (pos == e::first(x) + 1);

        pos = e::search_binary_upper(e::first(x), e::limit(x), 3);
        REQUIRE (pos == e::first(x) + 4);

        auto range = e::search_binary(e::first(x), e::limit(x), 3);
        REQUIRE (e::first(range) == e::first(x) + 1);
        REQUIRE (e::limit(range) == e::first(x) + 4);
    }

    SECTION ("Array with three 3s in the second half")
    {
        e::array<int> x{0, 1, 1, 2, 2, 3, 3, 3, 4};

        auto pos = e::search_binary_lower(e::first(x), e::limit(x), 3);
        REQUIRE (pos == e::first(x) + 5);

        pos = e::search_binary_upper(e::first(x), e::limit(x), 3);
        REQUIRE (pos == e::first(x) + 8);

        auto range = e::search_binary(e::first(x), e::limit(x), 3);
        REQUIRE (e::first(range) == e::first(x) + 5);
        REQUIRE (e::limit(range) == e::first(x) + 8);
    }
}
