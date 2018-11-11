#include "catch.hpp"

#include <chrono>
#include <complex>
#include <cstddef>

#include "elements.h"

namespace e = elements;

template <e::Ordered_additive_group G>
constexpr auto
property_abs(G const& a, G const& b) -> bool
{
    if (a < G{0} and G{0} >= -a) return false;
    if (b < G{0} and G{0} >= -b) return false;
    if (e::abs(a - b) != e::abs(b - a)) return false;
    if (e::abs(a + b) > e::abs(a) + e::abs(b)) return false;
    if (e::abs(a) == G{0} and a != G{0}) return false;
    if (a != G{0} and e::abs(a) <= G{0}) return false;
    return true;
}

SCENARIO ("Using abs", "[ordered_algebra]")
{
    SECTION ("Integers")
    {
        REQUIRE (e::abs(-1) == 1);
        REQUIRE (e::abs(0) == 0);
        REQUIRE (e::abs(1) == 1);
        REQUIRE (property_abs(-1, 0));
        REQUIRE (property_abs(-1, 1));
        REQUIRE (property_abs(0, -1));
        REQUIRE (property_abs(0, 1));
        REQUIRE (property_abs(1, -1));
        REQUIRE (property_abs(1, 0));
    }
}
