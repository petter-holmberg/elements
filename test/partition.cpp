#include "catch.hpp"

#include "elements.h"

namespace e = elements;

SCENARIO ("Partition points", "[partition]")
{
    auto is_even = [](int i){ return i % 2 == 0; };

    SECTION ("Checking if an empty array is partitioned")
    {
        e::array<int> x;

        REQUIRE (e::is_partitioned(e::first(x), e::limit(x), is_even));
        REQUIRE (!e::precedes(e::partition_point(e::first(x), e::limit(x), is_even), e::limit(x)));
    }

    SECTION ("Checking if a single-element array with an even number is partitioned")
    {
        int x[]{0};

        REQUIRE (e::is_partitioned(x, x + 1, is_even));
    }

    SECTION ("Checking if a single-element array with an odd number is partitioned")
    {
        int x[]{1};

        REQUIRE (e::is_partitioned(x, x + 1, is_even));
    }

    SECTION ("Checking if an array with an even and an odd number is partitioned")
    {
        int x[]{0, 1};

        REQUIRE (!e::is_partitioned(x, x + 2, is_even));
    }

    SECTION ("Checking if an array with an odd and an even number is partitioned")
    {
        int x[]{1, 0};

        REQUIRE (e::is_partitioned(x, x + 2, is_even));
    }
}
