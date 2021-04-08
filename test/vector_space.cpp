#include "catch.hpp"

#include <chrono>

#include "vector_space.h"

namespace e = elements;

SCENARIO ("Using vector spaces", "[vector_space]")
{
    static_assert(e::Vector_space<int>);
    static_assert(e::Vector_space<std::ptrdiff_t>);
}

SCENARIO ("Using vectors", "[vector]")
{
    SECTION ("Static vectors")
    {
        e::static_vector<int, 2> x;
        x(0) = 1;
        x(1) = 2;

        static_assert(e::Vector_space<decltype(x)>);
        static_assert(e::Mutable_range<decltype(x)>);
        static_assert(!e::Is_dynamic_vector<decltype(x)>);

        SECTION ("Vector algebra")
        {
            e::static_vector<int, 2> y;

            REQUIRE (y(0) == 0);
            REQUIRE (y(1) == 0);

            y = -x;

            REQUIRE (y(0) == -1);
            REQUIRE (y(1) == -2);

            e::static_vector<int, 2> z;
            z(0) = 3;
            z(1) = 4;

            y = x + z;

            REQUIRE (y(0) == 4);
            REQUIRE (y(1) == 6);

            y = x - z;

            REQUIRE (y(0) == -2);
            REQUIRE (y(1) == -2);

            y = x * 2;

            REQUIRE (y(0) == 2);
            REQUIRE (y(1) == 4);

            y = 2 * x;

            REQUIRE (y(0) == 2);
            REQUIRE (y(1) == 4);

            auto s = x * z;

            REQUIRE (s == 11);
        }
    }

    SECTION ("Dynamic vectors")
    {
        e::dynamic_vector<int> x{2, 0};
        x(0) = 1;
        x(1) = 2;

        static_assert(e::Vector_space<decltype(x)>);
        static_assert(e::Mutable_range<decltype(x)>);
        static_assert(e::Is_dynamic_vector<decltype(x)>);

        SECTION ("Vector algebra")
        {
            e::dynamic_vector<int> y;
            REQUIRE (e::is_empty(y));

            y = -x;

            REQUIRE (e::size(y) == e::size(x));
            REQUIRE (y(0) == -1);
            REQUIRE (y(1) == -2);

            e::dynamic_vector<int> z{2, 0};
            z(0) = 3;
            z(1) = 4;

            y = x + z;

            REQUIRE (e::size(y) == e::size(x));
            REQUIRE (y(0) == 4);
            REQUIRE (y(1) == 6);

            y = x - z;

            REQUIRE (e::size(y) == e::size(x));
            REQUIRE (y(0) == -2);
            REQUIRE (y(1) == -2);

            y = x * 2;

            REQUIRE (e::size(y) == e::size(x));
            REQUIRE (y(0) == 2);
            REQUIRE (y(1) == 4);

            y = 2 * x;

            REQUIRE (e::size(y) == e::size(x));
            REQUIRE (y(0) == 2);
            REQUIRE (y(1) == 4);

            auto s = x * z;

            REQUIRE (s == 11);
        }
    }
}
