#include "catch.hpp"

#include <future>

#include "locked_queue.h"
#include "array_double_ended.h"

namespace e = elements;

void async_push(e::locked_queue<e::array_double_ended<int>>* q, int x)
{
    e::push(*q, x);
}

SCENARIO ("Using locked queue", "[locked_queue]")
{
    SECTION ("Push and pop")
    {
        e::locked_queue<e::array_double_ended<int>> q;

        int x;
        REQUIRE (!try_pop(q, x));

        e::push(q, 0);
        e::push(q, 1);
        e::push(q, 2);

        REQUIRE (try_pop(q, x));
        REQUIRE (x == 0);

        pop(q, x);
        REQUIRE (x == 1);

        REQUIRE (try_pop(q, x));
        REQUIRE (x == 2);

        auto ans = std::async(std::launch::async, async_push, &q, 3);

        pop(q, x);
        REQUIRE (x == 3);

        REQUIRE (!try_pop(q, x));
    }
}
