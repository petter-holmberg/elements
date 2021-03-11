#include "catch.hpp"

#include "gather.h"

namespace e = elements;

SCENARIO ("Gathering elements", "[gather]")
{
    auto is_odd = [](int i){ return i % 2 == 1; };

    SECTION ("Unstable gathering")
    {
        int x[]{0, 1, 2, 3, 4, 5};

        SECTION ("Forward cursor")
        {
            {
                auto rng = e::gather_unstable(e::forward_cursor{x}, e::forward_cursor{x + 6}, e::forward_cursor{x}, is_odd);
                REQUIRE (e::first(rng) == e::forward_cursor{x});
                REQUIRE (e::limit(rng) == e::forward_cursor{x + 3});

                REQUIRE (x[0] == 1);
                REQUIRE (x[1] == 3);
                REQUIRE (x[2] == 5);
                REQUIRE (x[3] == 0);
                REQUIRE (x[4] == 4);
                REQUIRE (x[5] == 2);
            }

            {
                auto rng = e::gather_unstable(e::forward_cursor{x}, e::forward_cursor{x + 6}, e::forward_cursor{x + 4}, is_odd);
                REQUIRE (e::first(rng) == e::forward_cursor{x + 1});
                REQUIRE (e::limit(rng) == e::forward_cursor{x + 4});

                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 3);
                REQUIRE (x[2] == 5);
                REQUIRE (x[3] == 1);
                REQUIRE (x[4] == 4);
                REQUIRE (x[5] == 2);
            }

            {
                auto rng = e::gather_unstable(e::forward_cursor{x}, e::forward_cursor{x + 6}, e::forward_cursor{x + 6}, is_odd);
                REQUIRE (e::first(rng) == e::forward_cursor{x + 3});
                REQUIRE (e::limit(rng) == e::forward_cursor{x + 6});

                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 4);
                REQUIRE (x[2] == 2);
                REQUIRE (x[3] == 1);
                REQUIRE (x[4] == 3);
                REQUIRE (x[5] == 5);
            }
        }

        SECTION ("Bidirectional cursor")
        {
            {
                auto rng = e::gather_unstable(x, x + 6, x, is_odd);
                REQUIRE (e::first(rng) == x);
                REQUIRE (e::limit(rng) == x + 3);

                REQUIRE (x[0] == 5);
                REQUIRE (x[1] == 1);
                REQUIRE (x[2] == 3);
                REQUIRE (x[3] == 2);
                REQUIRE (x[4] == 4);
                REQUIRE (x[5] == 0);
            }

            {
                auto rng = e::gather_unstable(x, x + 6, x + 4, is_odd);
                REQUIRE (e::first(rng) == x + 1);
                REQUIRE (e::limit(rng) == x + 4);

                REQUIRE (x[0] == 2);
                REQUIRE (x[1] == 1);
                REQUIRE (x[2] == 3);
                REQUIRE (x[3] == 5);
                REQUIRE (x[4] == 4);
                REQUIRE (x[5] == 0);
            }

            {
                auto rng = e::gather_unstable(x, x + 6, x + 6, is_odd);
                REQUIRE (e::first(rng) == x + 3);
                REQUIRE (e::limit(rng) == x + 6);

                REQUIRE (x[0] == 2);
                REQUIRE (x[1] == 0);
                REQUIRE (x[2] == 4);
                REQUIRE (x[3] == 5);
                REQUIRE (x[4] == 3);
                REQUIRE (x[5] == 1);
            }
        }
    }

    SECTION ("Stable gathering")
    {
        int x[]{0, 1, 2, 3, 4, 5};
        int buf[]{0, 0, 0, 0, 0, 0};

        SECTION ("Forward cursor")
        {
            {
                auto rng = e::gather_stable_with_buffer(x, x + 6, x, buf, is_odd);
                REQUIRE (e::first(rng) == x);
                REQUIRE (e::limit(rng) == x + 3);

                REQUIRE (x[0] == 1);
                REQUIRE (x[1] == 3);
                REQUIRE (x[2] == 5);
                REQUIRE (x[3] == 0);
                REQUIRE (x[4] == 2);
                REQUIRE (x[5] == 4);
            }

            {
                auto rng = e::gather_stable_with_buffer(x, x + 6, x + 4, buf, is_odd);
                REQUIRE (e::first(rng) == x + 1);
                REQUIRE (e::limit(rng) == x + 4);

                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 1);
                REQUIRE (x[2] == 3);
                REQUIRE (x[3] == 5);
                REQUIRE (x[4] == 2);
                REQUIRE (x[5] == 4);
            }

            {
                auto rng = e::gather_stable_with_buffer(x, x + 6, x + 6, buf, is_odd);
                REQUIRE (e::first(rng) == x + 3);
                REQUIRE (e::limit(rng) == x + 6);

                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 2);
                REQUIRE (x[2] == 4);
                REQUIRE (x[3] == 1);
                REQUIRE (x[4] == 3);
                REQUIRE (x[5] == 5);
            }
        }

        SECTION ("Bidirectional cursor")
        {
            {
                auto rng = e::gather_stable(x, x + 6, x, is_odd);
                REQUIRE (e::first(rng) == x);
                REQUIRE (e::limit(rng) == x + 3);

                REQUIRE (x[0] == 1);
                REQUIRE (x[1] == 3);
                REQUIRE (x[2] == 5);
                REQUIRE (x[3] == 0);
                REQUIRE (x[4] == 2);
                REQUIRE (x[5] == 4);
            }

            {
                auto rng = e::gather_stable(x, x + 6, x + 4, is_odd);
                REQUIRE (e::first(rng) == x + 1);
                REQUIRE (e::limit(rng) == x + 4);

                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 1);
                REQUIRE (x[2] == 3);
                REQUIRE (x[3] == 5);
                REQUIRE (x[4] == 2);
                REQUIRE (x[5] == 4);
            }

            {
                auto rng = e::gather_stable(x, x + 6, x + 6, is_odd);
                REQUIRE (e::first(rng) == x + 3);
                REQUIRE (e::limit(rng) == x + 6);

                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 2);
                REQUIRE (x[2] == 4);
                REQUIRE (x[3] == 1);
                REQUIRE (x[4] == 3);
                REQUIRE (x[5] == 5);
            }
        }
    }
}
