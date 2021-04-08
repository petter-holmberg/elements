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

SCENARIO ("Using points", "[point]")
{
    e::point p{e::array_k<int, 2>{1, 2}};
    static_assert(e::Affine_space<decltype(p)>);

    SECTION ("Affine transformations")
    {
        e::point<int, 2> q;

        REQUIRE (q(0) == 0);
        REQUIRE (q(1) == 0);

        e::Difference_type<decltype(p)> v = p - q;

        REQUIRE (v(0) == 1);
        REQUIRE (v(1) == 2);

        p = p + v;

        REQUIRE (p(0) == 2);
        REQUIRE (p(1) == 4);

        p = v + p;

        REQUIRE (p(0) == 3);
        REQUIRE (p(1) == 6);

        p = p - v;

        REQUIRE (p(0) == 2);
        REQUIRE (p(1) == 4);
    }

    SECTION ("Quadrance")
    {
        e::point p0{e::array_k<int, 2>{0, 0}};
        e::point p1{e::array_k<int, 2>{4, 0}};
        e::point p2{e::array_k<int, 2>{4, 3}};

        REQUIRE (e::quadrance(p0, p1) == 16);
        REQUIRE (e::quadrance(p1, p0) == 16);
        REQUIRE (e::quadrance(p1, p2) == 9);
        REQUIRE (e::quadrance(p2, p1) == 9);
        REQUIRE (e::quadrance(p2, p0) == 25);
        REQUIRE (e::quadrance(p0, p2) == 25);
    }
}
