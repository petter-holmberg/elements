#include "catch.hpp"

#include "locked_stack.h"
#include "array_single_ended.h"

namespace e = elements;

SCENARIO ("Using locked stack", "[locked_stack]")
{
    SECTION ("Push and pop")
    {
        e::locked_stack<e::array_single_ended<int>> x;

        auto res{pop(x)};
        REQUIRE (!has_value(res));

        e::push(x, 0);
        e::push(x, 1);
        e::push(x, 2);

        res = pop(x);
        REQUIRE (has_value(res));
        REQUIRE (load(res) == 2);

        res = pop(x);
        REQUIRE (has_value(res));
        REQUIRE (load(res) == 1);

        res = pop(x);
        REQUIRE (has_value(res));
        REQUIRE (load(res) == 0);

        res = pop(x);
        REQUIRE (!has_value(res));
    }
}
