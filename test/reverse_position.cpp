#include "catch.hpp"

#include "elements.h"

namespace e = elements;

SCENARIO ("Reversing positions", "[reverse_position]")
{
    int xs[] = {0, 1};

    SECTION ("Bidirectional traversal")
    {
        auto px = e::reverse_position(xs + 2);
        static_assert(e::Bidirectional_position<decltype(px)>);
        static_assert(e::Affine_space<decltype(px)>);

        REQUIRE (load(px) == 1);
        REQUIRE (successor(px).pos == xs + 1);

        increment(px);
        REQUIRE (load(px) == 0);
        REQUIRE (predecessor(px).pos == xs + 2);

        decrement(px);
        REQUIRE (px.pos == xs + 2);
    }
}
