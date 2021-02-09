#include "catch.hpp"

#include "list_pool.h"

namespace e = elements;

SCENARIO ("List pool", "[list_pool]")
{
    SECTION ("Empty list pool of integers")
    {
        e::list_pool<int> x;

        REQUIRE (size(x) == 0);
        REQUIRE (capacity(x) == 0);
    }

    SECTION ("Inserting elements into pool")
    {
        e::list_pool<int, int> x;
        auto tail = allocate(x, 1, x.limit());
        tail = allocate(x, 2, tail);
        tail = allocate(x, 3, tail);

        REQUIRE (size(x) == 3);
        REQUIRE (capacity(x) != 0);

        REQUIRE (value(x, 1) == 1);
        REQUIRE (next(x, 1) == x.limit());
        REQUIRE (value(x, 2) == 2);
        REQUIRE (value(x, next(x, 2)) == 1);
        REQUIRE (value(x, 3) == 3);
        REQUIRE (value(x, next(x, 3)) == 2);

        SECTION ("Iterating over elements")
        {
            e::list_pool_cursor pos{x, 3};
            e::list_pool_cursor lim{x};

            REQUIRE(load(pos) == 3);
            REQUIRE(value(x, next_link(pos)) == 2);

            value(x, next_link(pos)) = -2;

            increment(pos);

            REQUIRE(load(pos) == -2);

            increment(pos);

            REQUIRE(load(pos) == 1);

            increment(pos);

            REQUIRE(pos == lim);
        }

        SECTION ("Removing 3 elements") {
            free_pool(x, 3);

            REQUIRE (size(x) == 3);
            REQUIRE (capacity(x) != 0);

            REQUIRE (next(x, 1) == 2);
            REQUIRE (next(x, 2) == 3);
            REQUIRE (next(x, 3) == 0);
        }

        SECTION ("Removing 2 elements") {
            free_pool(x, 2);

            REQUIRE (size(x) == 3);
            REQUIRE (capacity(x) != 0);

            REQUIRE (next(x, 1) == 2);
            REQUIRE (next(x, 2) == 0);
            REQUIRE (next(x, 3) == 2);
        }

        SECTION ("Removing 1 element") {
            free_pool(x, 1);

            REQUIRE (size(x) == 3);
            REQUIRE (capacity(x) != 0);

            REQUIRE (next(x, 1) == 0);
            REQUIRE (next(x, 2) == 1);
            REQUIRE (next(x, 3) == 2);
        }
    }
}
