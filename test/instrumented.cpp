#include "catch.hpp"

#include "instrumented.h"

namespace e = elements;

SCENARIO ("Instrumented", "[instrumented]")
{
    static_assert(e::Same_as<e::Value_type<e::instrumented<int>>, int>);

    e::instrumented<int>::initialize(0);

    SECTION ("Initialization")
    {
        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::n] == 0);

        e::instrumented<int>::initialize(1);

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::n] == 1);
    }

    SECTION ("Default construction")
    {
        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::default_construct] == 0);

        e::instrumented<int> x;

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::default_construct] == 1);

        e::instrumented<int> y;

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::default_construct] == 2);

        e::instrumented<int>::initialize(0);

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::default_construct] == 0);
    }

    SECTION ("Copy construction")
    {
        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::copy] == 0);

        e::instrumented<int> x;
        e::instrumented<int> y{x};

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::copy] == 1);

        e::instrumented<int> z{y};

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::copy] == 2);

        e::instrumented<int>::initialize(0);

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::copy] == 0);
    }

    SECTION ("Copy assignment")
    {
        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::assign] == 0);

        e::instrumented<int> x;
        e::instrumented<int> y;
        y = x;

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::assign] == 1);

        e::instrumented<int> z;
        z = y;

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::assign] == 2);

        e::instrumented<int>::initialize(0);

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::assign] == 0);
    }

    SECTION ("Move construction")
    {
        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::move] == 0);

        e::instrumented<int> x;
        e::instrumented<int> y{e::mv(x)};

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::move] == 1);

        e::instrumented<int> z{e::mv(y)};

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::move] == 2);

        e::instrumented<int>::initialize(0);

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::move] == 0);
    }

    SECTION ("Move assignment")
    {
        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::move_assign] == 0);

        e::instrumented<int> x;
        e::instrumented<int> y;
        y = e::mv(x);

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::move_assign] == 1);

        e::instrumented<int> z;
        z = e::mv(y);

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::move_assign] == 2);

        e::instrumented<int>::initialize(0);

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::move_assign] == 0);
    }

    SECTION ("Destruction")
    {
        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::destruct] == 0);

        {
            e::instrumented<int> x;
        }

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::destruct] == 1);

        {
            e::instrumented<int> x;
        }

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::destruct] == 2);

        e::instrumented<int>::initialize(0);

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::destruct] == 0);
    }

    SECTION ("Equality comparison")
    {
        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::equal] == 0);

        e::instrumented<int> x;
        e::instrumented<int> y;
        x == y;

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::equal] == 1);

        e::instrumented<int> z;
        z == y;

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::equal] == 2);

        e::instrumented<int>::initialize(0);

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::equal] == 0);
    }

    SECTION ("Less comparison")
    {
        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::less] == 0);

        e::instrumented<int> x;
        e::instrumented<int> y;
        x < y;

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::less] == 1);

        e::instrumented<int> z;
        z < y;

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::less] == 2);

        e::instrumented<int>::initialize(0);

        REQUIRE (e::instrumented<int>::counts[e::instrumented<int>::less] == 0);
    }
}
