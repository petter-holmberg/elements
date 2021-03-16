#include "catch.hpp"

#include "polynomial.h"

namespace e = elements;

SCENARIO ("Using polynomials", "[polynomial]")
{

    e::array_single_ended<int> a;
    e::emplace(a, 1);
    e::emplace(a, 2);

    e::polynomial x{mv(a)};

    static_assert(e::Integral_domain<decltype(x)>);

    SECTION ("Algebra")
    {
        e::polynomial<int> p0;

        e::array_single_ended<int> a1;
        e::emplace(a1, 1);
        e::emplace(a1, 2);
        e::polynomial p1{mv(a1)};

        e::array_single_ended<int> a2;
        e::emplace(a2, 5);
        e::emplace(a2, 2);
        e::emplace(a2, 0);
        e::emplace(a2, 3);
        e::polynomial p2{mv(a2)};

        SECTION ("Total ordering")
        {
            REQUIRE (e::axiom_Totally_ordered(p0, p1));
            REQUIRE (e::axiom_Totally_ordered(p1, p2));

            e::array_single_ended<int> a3;
            e::emplace(a3, 5);
            e::emplace(a3, 2);
            e::emplace(a3, 0);
            e::emplace(a3, 3);
            e::emplace(a3, 0);
            e::polynomial p3{mv(a3)};

            REQUIRE (p2 == p3);
            REQUIRE (p2 <= p3);
            REQUIRE (p2 >= p3);
        }

        SECTION ("Degree")
        {
            e::array_single_ended<int> a3;
            e::emplace(a3, 6);
            e::emplace(a3, 4);
            e::emplace(a3, 0);
            e::emplace(a3, 3);
            e::emplace(a3, 0);
            e::emplace(a3, 0);
            e::polynomial p3{mv(a3)};

            REQUIRE (degree(p0) < 0);
            REQUIRE (degree(p1) == 1);
            REQUIRE (degree(p2) == 3);
            REQUIRE (degree(p3) == 3);
        }

        SECTION ("Evaluation")
        {
            REQUIRE (evaluate(p2, 0) == 5);
            REQUIRE (evaluate(p2, 1) == 10);
            REQUIRE (evaluate(p2, 2) == 33);
        }

        SECTION ("Addition")
        {
            e::array_single_ended<int> a3;
            e::emplace(a3, 6);
            e::emplace(a3, 4);
            e::emplace(a3, 0);
            e::emplace(a3, 3);
            e::polynomial p3{mv(a3)};

            REQUIRE (p1 + p2 == p3);
            REQUIRE (axiom_Additive_group(p1, p2, p3));
        }

        SECTION ("Subtraction")
        {
            e::array_single_ended<int> a3;
            e::emplace(a3, -4);
            e::emplace(a3, 0);
            e::emplace(a3, 0);
            e::emplace(a3, -3);
            e::polynomial p3{mv(a3)};

            REQUIRE (p1 - p2 == p3);
        }

        SECTION ("Multiplication")
        {
            e::array_single_ended<int> a3;
            e::emplace(a3, 5);
            e::emplace(a3, 12);
            e::emplace(a3, 4);
            e::emplace(a3, 3);
            e::emplace(a3, 6);
            e::polynomial p3{mv(a3)};

            REQUIRE (p1 * p2 == p3);
            REQUIRE (p2 * p1 == p3);
            REQUIRE (axiom_Multiplicative_monoid(p1, p2, p3));
            REQUIRE (axiom_Ring(p0, p1, p2));
        }

        SECTION ("Scalar multiplication")
        {
            e::array_single_ended<int> a3;
            e::emplace(a3, 2);
            e::emplace(a3, 4);
            e::polynomial p3{mv(a3)};

            REQUIRE (p1 * 2 == p3);

            // Commutative
            REQUIRE (p1 * 2 == 2 * p1);
        }

        SECTION ("Subderivatives")
        {
            e::array_single_ended<int> a3;
            e::emplace(a3, 8);
            e::emplace(a3, -5);
            e::emplace(a3, 0);
            e::emplace(a3, 4);
            e::emplace(a3, -1);
            e::polynomial p3{mv(a3)};

            e::array_single_ended<int> a4;
            e::emplace(a4, -5);
            e::emplace(a4, 0);
            e::emplace(a4, 12);
            e::emplace(a4, -4);
            e::polynomial p4{mv(a4)};

            e::array_single_ended<int> a5;
            e::emplace(a5, 0);
            e::emplace(a5, 12);
            e::emplace(a5, -6);
            e::polynomial p5{mv(a5)};

            e::array_single_ended<int> a6;
            e::emplace(a6, 4);
            e::emplace(a6, -4);
            e::polynomial p6{mv(a6)};

            e::array_single_ended<int> a7;
            e::emplace(a7, -1);
            e::polynomial p7{mv(a7)};

            REQUIRE (e::subderivative(p3, 0) == p3);
            REQUIRE (e::subderivative(p3, 1) == p4);
            REQUIRE (e::subderivative(p3, 2) == p5);
            REQUIRE (e::subderivative(p3, 3) == p6);
            REQUIRE (e::subderivative(p3, 4) == p7);
        }
    }
}
