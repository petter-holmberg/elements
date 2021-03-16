#include "catch.hpp"

#include "locked_stack.h"
#include "array_single_ended.h"

namespace e = elements;

SCENARIO ("Using locked stack", "[locked_stack]")
{
    SECTION ("Push and pop")
    {
        e::locked_stack<e::array_single_ended<int>> s;

        int x;
        REQUIRE (!try_pop(s, x));

        e::push(s, 0);
        e::push(s, 1);
        e::push(s, 2);

        REQUIRE (try_pop(s, x));
        REQUIRE (x == 2);

        REQUIRE (try_pop(s, x));
        REQUIRE (x == 1);

        REQUIRE (try_pop(s, x));
        REQUIRE (x == 0);

        REQUIRE (!try_pop(s, x));
        REQUIRE (x == 0);
    }
}
