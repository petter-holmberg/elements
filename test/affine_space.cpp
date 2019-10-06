#include "catch.hpp"

#include <chrono>

#include "affine_space.h"

namespace e = elements;

SCENARIO ("Using affine spaces", "[affine_space]")
{
    static_assert(e::Affine_space<int*>);

    static_assert(
            e::Affine_space<
                std::chrono::system_clock::time_point,
                typename std::chrono::system_clock::time_point::duration,
                typename std::chrono::system_clock::time_point::duration::rep>);
}

SCENARIO ("Using affine vectors", "[affine_vector]")
{
    e::affine_vector<int, 2> x{1, 2};
    static_assert(e::Vector_space<decltype(x)>);
    static_assert(e::Sequence<decltype(x)>);

    SECTION ("Vector algebra")
    {
        e::affine_vector<int, 2> z;

        REQUIRE (z[0] == 0);
        REQUIRE (z[1] == 0);

        z = -x;

        REQUIRE (z[0] == -1);
        REQUIRE (z[1] == -2);

        e::affine_vector<int, 2> y{3, 4};

        z = x + y;

        REQUIRE (z[0] == 4);
        REQUIRE (z[1] == 6);

        z = x - y;

        REQUIRE (z[0] == -2);
        REQUIRE (z[1] == -2);

        z = x * 2;

        REQUIRE (z[0] == 2);
        REQUIRE (z[1] == 4);

        z = y / 2;

        REQUIRE (z[0] == 1);
        REQUIRE (z[1] == 2);

        REQUIRE (x * y == 11);
        REQUIRE (y * x == 11);
    }
}

SCENARIO ("Using affine points", "[affine_point]")
{
    e::affine_point<int, 2> x{1, 2};
    static_assert(e::Affine_space<decltype(x)>);
    static_assert(e::Sequence<decltype(x)>);

    SECTION ("Affine transformations")
    {
        e::affine_point<int, 2> z;

        REQUIRE (z[0] == 0);
        REQUIRE (z[1] == 0);

        e::Difference_type<decltype(x)> v = x - z;

        REQUIRE (v[0] == 1);
        REQUIRE (v[1] == 2);

        x = x + v;

        REQUIRE (x[0] == 2);
        REQUIRE (x[1] == 4);

        x = v + x;

        REQUIRE (x[0] == 3);
        REQUIRE (x[1] == 6);

        x = x - v;

        REQUIRE (x[0] == 2);
        REQUIRE (x[1] == 4);
    }
}
