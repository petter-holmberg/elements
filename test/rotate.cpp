#include "catch.hpp"

#include "rotate.h"

namespace e = elements;

SCENARIO ("Using rotate", "[rotate]")
{
    int x[]{0, 1, 2, 3, 4};

    SECTION ("Forward cursor")
    {
        auto first = e::forward_cursor{x};
        auto limit = e::forward_cursor{x + 5};

        SECTION ("Rotating 0 steps")
        {
            auto cur = e::rotate(first, limit, first);

            REQUIRE (cur == limit);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);

            cur = e::rotate(first, limit, cur);

            REQUIRE (cur == first);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Rotating 1 step")
        {
            auto cur = e::rotate(first, limit, first + 1);

            REQUIRE (cur == first + 4);

            CHECK (x[0] == 1);
            CHECK (x[1] == 2);
            CHECK (x[2] == 3);
            CHECK (x[3] == 4);
            CHECK (x[4] == 0);

            cur = e::rotate(first, limit, cur);

            REQUIRE (cur == first + 1);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Rotating 2 steps")
        {
            auto cur = e::rotate(first, limit, first + 2);

            REQUIRE (cur == first + 3);

            CHECK (x[0] == 2);
            CHECK (x[1] == 3);
            CHECK (x[2] == 4);
            CHECK (x[3] == 0);
            CHECK (x[4] == 1);

            cur = e::rotate(first, limit, cur);

            REQUIRE (cur == first + 2);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Rotating 3 steps")
        {
            auto cur = e::rotate(first, limit, first + 3);

            REQUIRE (cur == first + 2);

            CHECK (x[0] == 3);
            CHECK (x[1] == 4);
            CHECK (x[2] == 0);
            CHECK (x[3] == 1);
            CHECK (x[4] == 2);

            cur = e::rotate(first, limit, cur);

            REQUIRE (cur == first + 3);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Rotating 4 steps")
        {
            auto cur = e::rotate(first, limit, first + 4);

            REQUIRE (cur == first + 1);

            CHECK (x[0] == 4);
            CHECK (x[1] == 0);
            CHECK (x[2] == 1);
            CHECK (x[3] == 2);
            CHECK (x[4] == 3);

            cur = e::rotate(first, limit, cur);

            REQUIRE (cur == first + 4);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Rotating 5 steps")
        {
            auto cur = e::rotate(first, limit, first + 5);

            REQUIRE (cur == first);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);

            cur = e::rotate(first, limit, cur);

            REQUIRE (cur == first + 5);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }
    }

    SECTION ("Bidirectional cursor")
    {
        auto first = e::bidirectional_cursor{x};
        auto limit = e::bidirectional_cursor{x + 5};

        SECTION ("Rotating 0 steps")
        {
            auto cur = e::rotate(first, limit, first);

            REQUIRE (cur == limit);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);

            cur = e::rotate(first, limit, cur);

            REQUIRE (cur == first);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Rotating 1 step")
        {
            auto cur = e::rotate(first, limit, first + 1);

            REQUIRE (cur == first + 4);

            CHECK (x[0] == 1);
            CHECK (x[1] == 2);
            CHECK (x[2] == 3);
            CHECK (x[3] == 4);
            CHECK (x[4] == 0);

            cur = e::rotate(first, limit, cur);

            REQUIRE (cur == first + 1);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Rotating 2 steps")
        {
            auto cur = e::rotate(first, limit, first + 2);

            REQUIRE (cur == first + 3);

            CHECK (x[0] == 2);
            CHECK (x[1] == 3);
            CHECK (x[2] == 4);
            CHECK (x[3] == 0);
            CHECK (x[4] == 1);

            cur = e::rotate(first, limit, cur);

            REQUIRE (cur == first + 2);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Rotating 3 steps")
        {
            auto cur = e::rotate(first, limit, first + 3);

            REQUIRE (cur == first + 2);

            CHECK (x[0] == 3);
            CHECK (x[1] == 4);
            CHECK (x[2] == 0);
            CHECK (x[3] == 1);
            CHECK (x[4] == 2);

            cur = e::rotate(first, limit, cur);

            REQUIRE (cur == first + 3);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Rotating 4 steps")
        {
            auto cur = e::rotate(first, limit, first + 4);

            REQUIRE (cur == first + 1);

            CHECK (x[0] == 4);
            CHECK (x[1] == 0);
            CHECK (x[2] == 1);
            CHECK (x[3] == 2);
            CHECK (x[4] == 3);

            cur = e::rotate(first, limit, cur);

            REQUIRE (cur == first + 4);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }

        SECTION ("Rotating 5 steps")
        {
            auto cur = e::rotate(first, limit, first + 5);

            REQUIRE (cur == first);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);

            cur = e::rotate(first, limit, cur);

            REQUIRE (cur == first + 5);

            CHECK (x[0] == 0);
            CHECK (x[1] == 1);
            CHECK (x[2] == 2);
            CHECK (x[3] == 3);
            CHECK (x[4] == 4);
        }
    }
}
