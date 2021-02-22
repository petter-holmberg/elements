#include "catch.hpp"

#include "ordered_algebra.h"
#include "rational.h"

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

SCENARIO ("Calculating absolute value", "[ordered_algebra]")
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

SCENARIO ("Calculating quotient and remainder", "[ordered_algebra]")
{
    SECTION ("Integer quotient and remainder")
    {
        REQUIRE (e::quotient_remainder(0, 1).m0 == 0);
        REQUIRE (e::quotient_remainder(0, 1).m1 == 0);
        REQUIRE (e::quotient_remainder(1, 1).m0 == 1);
        REQUIRE (e::quotient_remainder(1, 1).m1 == 0);
        REQUIRE (e::quotient_remainder(1, 2).m0 == 0);
        REQUIRE (e::quotient_remainder(1, 2).m1 == 1);
        REQUIRE (e::quotient_remainder(2, 1).m0 == 2);
        REQUIRE (e::quotient_remainder(2, 1).m1 == 0);
        REQUIRE (e::quotient_remainder(2, 2).m0 == 1);
        REQUIRE (e::quotient_remainder(2, 2).m1 == 0);
        REQUIRE (e::quotient_remainder(2, 3).m0 == 0);
        REQUIRE (e::quotient_remainder(2, 3).m1 == 2);
        REQUIRE (e::quotient_remainder(83, 3).m0 == 27);
        REQUIRE (e::quotient_remainder(83, 3).m1 == 2);
    }

    SECTION ("Rational logarithm and remainder")
    {
        REQUIRE (e::quotient_remainder(e::rational{0, 1}, e::rational{2, 1}, e::multiply).m0 == e::rational{0, 1});
        REQUIRE (e::quotient_remainder(e::rational{0, 1}, e::rational{2, 1}, e::multiply).m1 == e::rational{0, 1});
        REQUIRE (e::quotient_remainder(e::rational{1, 1}, e::rational{2, 1}, e::multiply).m0 == e::rational{0, 1});
        REQUIRE (e::quotient_remainder(e::rational{1, 1}, e::rational{2, 1}, e::multiply).m1 == e::rational{1, 1});
        REQUIRE (e::quotient_remainder(e::rational{1, 1}, e::rational{3, 1}, e::multiply).m0 == e::rational{0, 1});
        REQUIRE (e::quotient_remainder(e::rational{1, 1}, e::rational{3, 1}, e::multiply).m1 == e::rational{1, 1});
        REQUIRE (e::quotient_remainder(e::rational{2, 1}, e::rational{2, 1}, e::multiply).m0 == e::rational{1, 1});
        REQUIRE (e::quotient_remainder(e::rational{2, 1}, e::rational{2, 1}, e::multiply).m1 == e::rational{1, 1});
        REQUIRE (e::quotient_remainder(e::rational{2, 1}, e::rational{3, 1}, e::multiply).m0 == e::rational{0, 1});
        REQUIRE (e::quotient_remainder(e::rational{2, 1}, e::rational{3, 1}, e::multiply).m1 == e::rational{2, 1});
        REQUIRE (e::quotient_remainder(e::rational{2, 1}, e::rational{4, 1}, e::multiply).m0 == e::rational{0, 1});
        REQUIRE (e::quotient_remainder(e::rational{2, 1}, e::rational{4, 1}, e::multiply).m1 == e::rational{2, 1});
        REQUIRE (e::quotient_remainder(e::rational{83, 1}, e::rational{3, 1}, e::multiply).m0 == e::rational{4, 1});
        REQUIRE (e::quotient_remainder(e::rational{83, 1}, e::rational{3, 1}, e::multiply).m1 == e::rational{83, 81});
    }
}

SCENARIO ("Calculating greatest common divisor", "[ordered_algebra]")
{
    SECTION ("Integer gcd")
    {
        static_assert(e::Euclidean_semimodule<int>);

        REQUIRE (e::gcd(0, 0) == 0);
        REQUIRE (e::gcd(0, 1) == 1);
        REQUIRE (e::gcd(0, 2) == 2);
        REQUIRE (e::gcd(0, 3) == 3);
        REQUIRE (e::gcd(0, 4) == 4);
        REQUIRE (e::gcd(0, 5) == 5);
        REQUIRE (e::gcd(0, 6) == 6);

        REQUIRE (e::gcd(1, 0) == 1);
        REQUIRE (e::gcd(1, 1) == 1);
        REQUIRE (e::gcd(1, 2) == 1);
        REQUIRE (e::gcd(1, 3) == 1);
        REQUIRE (e::gcd(1, 4) == 1);
        REQUIRE (e::gcd(1, 5) == 1);
        REQUIRE (e::gcd(1, 6) == 1);

        REQUIRE (e::gcd(2, 0) == 2);
        REQUIRE (e::gcd(2, 1) == 1);
        REQUIRE (e::gcd(2, 2) == 2);
        REQUIRE (e::gcd(2, 3) == 1);
        REQUIRE (e::gcd(2, 4) == 2);
        REQUIRE (e::gcd(2, 5) == 1);
        REQUIRE (e::gcd(2, 6) == 2);

        REQUIRE (e::gcd(3, 0) == 3);
        REQUIRE (e::gcd(3, 1) == 1);
        REQUIRE (e::gcd(3, 2) == 1);
        REQUIRE (e::gcd(3, 3) == 3);
        REQUIRE (e::gcd(3, 4) == 1);
        REQUIRE (e::gcd(3, 5) == 1);
        REQUIRE (e::gcd(3, 6) == 3);

        REQUIRE (e::gcd(4, 0) == 4);
        REQUIRE (e::gcd(4, 1) == 1);
        REQUIRE (e::gcd(4, 2) == 2);
        REQUIRE (e::gcd(4, 3) == 1);
        REQUIRE (e::gcd(4, 4) == 4);
        REQUIRE (e::gcd(4, 5) == 1);
        REQUIRE (e::gcd(4, 6) == 2);

        REQUIRE (e::gcd(5, 0) == 5);
        REQUIRE (e::gcd(5, 1) == 1);
        REQUIRE (e::gcd(5, 2) == 1);
        REQUIRE (e::gcd(5, 3) == 1);
        REQUIRE (e::gcd(5, 4) == 1);
        REQUIRE (e::gcd(5, 5) == 5);
        REQUIRE (e::gcd(5, 6) == 1);

        REQUIRE (e::gcd(6, 0) == 6);
        REQUIRE (e::gcd(6, 1) == 1);
        REQUIRE (e::gcd(6, 2) == 2);
        REQUIRE (e::gcd(6, 3) == 3);
        REQUIRE (e::gcd(6, 4) == 2);
        REQUIRE (e::gcd(6, 5) == 1);
        REQUIRE (e::gcd(6, 6) == 6);
    }

    SECTION ("Rational gcd")
    {
        static_assert(e::Euclidean_semimodule<e::rational<int>>);

        REQUIRE (e::gcd(e::rational{1, 2}, e::rational{3, 4}) == e::rational{1, 4});
    }
}
