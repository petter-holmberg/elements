#include "catch.hpp"

#include "copy.h"

namespace e = elements;

SCENARIO ("Copying", "[copy]")
{
    int x[]{0, 1, 2, 3, 4};
    int y[]{5, 6, 7, 8, 9};

    SECTION ("Copying one array to another")
    {
        auto cur = e::copy(x, x + 5, y);

        REQUIRE(cur == y + 5);

        CHECK (y[0] == 0);
        CHECK (y[1] == 1);
        CHECK (y[2] == 2);
        CHECK (y[3] == 3);
        CHECK (y[4] == 4);
    }

    SECTION ("Copying an array into itelf")
    {
        auto cur = e::copy(x, x + 5, x);

        REQUIRE(cur == x + 5);

        CHECK (x[0] == 0);
        CHECK (x[1] == 1);
        CHECK (x[2] == 2);
        CHECK (x[3] == 3);
        CHECK (x[4] == 4);
    }

    SECTION ("Copying an array to a shorter array with destination bounds checking")
    {
        auto curs = e::copy(x, x + 5, y, y + 3);

        REQUIRE(e::get<0>(curs) == x + 3);
        REQUIRE(e::get<1>(curs) == y + 3);

        CHECK (y[0] == 0);
        CHECK (y[1] == 1);
        CHECK (y[2] == 2);
        CHECK (y[3] == 8);
        CHECK (y[4] == 9);
    }

    SECTION ("Copying one array to another, counted")
    {
        auto p = e::copy_n(x, 5, y);

        REQUIRE(p.m0 == x + 5);
        REQUIRE(p.m1 == y + 5);

        CHECK (y[0] == 0);
        CHECK (y[1] == 1);
        CHECK (y[2] == 2);
        CHECK (y[3] == 3);
        CHECK (y[4] == 4);
    }

    SECTION ("Copying a backward-overlapping sub-range of an array")
    {
        auto cur = e::copy(x + 1, x + 5, x);

        REQUIRE(cur == x + 4);

        CHECK (x[0] == 1);
        CHECK (x[1] == 2);
        CHECK (x[2] == 3);
        CHECK (x[3] == 4);
        CHECK (x[4] == 4);
    }

    SECTION ("Copying with predicate")
    {
        auto is_even = [](int i){ return i % 2 == 0; };

        SECTION ("Matching predicate")
        {
            auto cur = e::copy_if(x, x + 5, y, is_even);

            REQUIRE(cur == y + 3);
            CHECK (y[0] == 0);
            CHECK (y[1] == 2);
            CHECK (y[2] == 4);
            CHECK (y[3] == 8);
            CHECK (y[4] == 9);
        }

        SECTION ("Mismatching predicate")
        {
            auto cur = e::copy_if_not(x, x + 5, y, is_even);

            REQUIRE(cur == y + 2);
            CHECK (y[0] == 1);
            CHECK (y[1] == 3);
            CHECK (y[2] == 7);
            CHECK (y[3] == 8);
            CHECK (y[4] == 9);
        }
    }
}
