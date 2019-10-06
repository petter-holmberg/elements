#include "catch.hpp"

#include "rotate.h"

namespace e = elements;

SCENARIO ("Using rotate", "[rotate]")
{
    int x[]{0, 1, 2, 3, 4};

    SECTION ("Forward position")
    {
        auto first = e::forward_position{x};
        auto limit = e::forward_position{x + 5};

        SECTION ("Rotating 0 steps")
        {
            auto pos = e::rotate(first, limit, first);

            REQUIRE (pos == limit);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);

            pos = e::rotate(first, limit, pos);

            REQUIRE (pos == first);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Rotating 1 step")
        {
            auto pos = e::rotate(first, limit, first + 1);

            REQUIRE (pos == first + 4);

            CHECK (x[0] == 1);
            CHECK (x[1] == 2);
            CHECK (x[2] == 3);
            CHECK (x[3] == 4);
            CHECK (x[4] == 0);

            pos = e::rotate(first, limit, pos);

            REQUIRE (pos == first + 1);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Rotating 2 steps")
        {
            auto pos = e::rotate(first, limit, first + 2);

            REQUIRE (pos == first + 3);

            CHECK (x[0] == 2);
            CHECK (x[1] == 3);
            CHECK (x[2] == 4);
            CHECK (x[3] == 0);
            CHECK (x[4] == 1);

            pos = e::rotate(first, limit, pos);

            REQUIRE (pos == first + 2);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Rotating 3 steps")
        {
            auto pos = e::rotate(first, limit, first + 3);

            REQUIRE (pos == first + 2);

            CHECK (x[0] == 3);
            CHECK (x[1] == 4);
            CHECK (x[2] == 0);
            CHECK (x[3] == 1);
            CHECK (x[4] == 2);

            pos = e::rotate(first, limit, pos);

            REQUIRE (pos == first + 3);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Rotating 4 steps")
        {
            auto pos = e::rotate(first, limit, first + 4);

            REQUIRE (pos == first + 1);

            CHECK (x[0] == 4);
            CHECK (x[1] == 0);
            CHECK (x[2] == 1);
            CHECK (x[3] == 2);
            CHECK (x[4] == 3);

            pos = e::rotate(first, limit, pos);

            REQUIRE (pos == first + 4);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Rotating 5 steps")
        {
            auto pos = e::rotate(first, limit, first + 5);

            REQUIRE (pos == first);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);

            pos = e::rotate(first, limit, pos);

            REQUIRE (pos == first + 5);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }
    }

    SECTION ("Bidirectional position")
    {
        auto first = e::bidirectional_position{x};
        auto limit = e::bidirectional_position{x + 5};

        SECTION ("Rotating 0 steps")
        {
            auto pos = e::rotate(first, limit, first);

            REQUIRE (pos == limit);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);

            pos = e::rotate(first, limit, pos);

            REQUIRE (pos == first);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Rotating 1 step")
        {
            auto pos = e::rotate(first, limit, first + 1);

            REQUIRE (pos == first + 4);

            CHECK (x[0] == 1);
            CHECK (x[1] == 2);
            CHECK (x[2] == 3);
            CHECK (x[3] == 4);
            CHECK (x[4] == 0);

            pos = e::rotate(first, limit, pos);

            REQUIRE (pos == first + 1);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Rotating 2 steps")
        {
            auto pos = e::rotate(first, limit, first + 2);

            REQUIRE (pos == first + 3);

            CHECK (x[0] == 2);
            CHECK (x[1] == 3);
            CHECK (x[2] == 4);
            CHECK (x[3] == 0);
            CHECK (x[4] == 1);

            pos = e::rotate(first, limit, pos);

            REQUIRE (pos == first + 2);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Rotating 3 steps")
        {
            auto pos = e::rotate(first, limit, first + 3);

            REQUIRE (pos == first + 2);

            CHECK (x[0] == 3);
            CHECK (x[1] == 4);
            CHECK (x[2] == 0);
            CHECK (x[3] == 1);
            CHECK (x[4] == 2);

            pos = e::rotate(first, limit, pos);

            REQUIRE (pos == first + 3);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Rotating 4 steps")
        {
            auto pos = e::rotate(first, limit, first + 4);

            REQUIRE (pos == first + 1);

            CHECK (x[0] == 4);
            CHECK (x[1] == 0);
            CHECK (x[2] == 1);
            CHECK (x[3] == 2);
            CHECK (x[4] == 3);

            pos = e::rotate(first, limit, pos);

            REQUIRE (pos == first + 4);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Rotating 5 steps")
        {
            auto pos = e::rotate(first, limit, first + 5);

            REQUIRE (pos == first);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);

            pos = e::rotate(first, limit, pos);

            REQUIRE (pos == first + 5);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }
    }
}
