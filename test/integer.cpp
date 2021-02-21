#include "catch.hpp"

#include <complex>
#include <cstddef>

#include "integer.h"

namespace e = elements;

SCENARIO ("Calculating powers", "[integer]")
{
    SECTION ("Semigroups")
    {
        static_assert(e::power_semigroup(1, 1, e::sum{}) == 1);
        static_assert(e::power_semigroup(1, 2, e::sum{}) == 2);
        static_assert(e::power_semigroup(2, 1, e::sum{}) == 2);
        static_assert(e::power_semigroup(2, 2, e::sum{}) == 4);

        static_assert(e::power_semigroup(1, 1, e::product{}) == 1);
        static_assert(e::power_semigroup(1, 2, e::product{}) == 1);
        static_assert(e::power_semigroup(2, 1, e::product{}) == 2);
        static_assert(e::power_semigroup(2, 2, e::product{}) == 4);
    }

    SECTION ("Monoids")
    {
        static_assert(e::power_monoid(1, 0, e::sum{}) == 0);
        static_assert(e::power_monoid(1, 1, e::sum{}) == 1);
        static_assert(e::power_monoid(1, 2, e::sum{}) == 2);
        static_assert(e::power_monoid(2, 1, e::sum{}) == 2);
        static_assert(e::power_monoid(2, 2, e::sum{}) == 4);

        static_assert(e::power_monoid(1, 0, e::product{}) == 1);
        static_assert(e::power_monoid(1, 1, e::product{}) == 1);
        static_assert(e::power_monoid(1, 2, e::product{}) == 1);
        static_assert(e::power_monoid(2, 1, e::product{}) == 2);
        static_assert(e::power_monoid(2, 2, e::product{}) == 4);
    }

    SECTION ("Groups")
    {
        static_assert(e::power_group(2, -2, e::sum{}) == -4);
        static_assert(e::power_group(2, -1, e::sum{}) == -2);
        static_assert(e::power_group(1, -2, e::sum{}) == -2);
        static_assert(e::power_group(1, -1, e::sum{}) == -1);
        static_assert(e::power_group(1, 0, e::sum{}) == 0);
        static_assert(e::power_group(1, 1, e::sum{}) == 1);
        static_assert(e::power_group(1, 2, e::sum{}) == 2);
        static_assert(e::power_group(2, 1, e::sum{}) == 2);
        static_assert(e::power_group(2, 2, e::sum{}) == 4);
    }
}
