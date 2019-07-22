#include "catch.hpp"

#include "elements.h"

namespace e = elements;

SCENARIO ("Using polynomials", "[polynomial]")
{
   e::polynomial<int> x{1, 2};

   static_assert(e::Integral_domain<decltype(x)>);

    SECTION ("Algebra")
    {
        e::polynomial<int> p0;
        e::polynomial<int> p1{1, 2};
        e::polynomial<int> p2{5, 2, 0, 3};

        SECTION ("Total ordering")
        {
            REQUIRE (e::axiom_Totally_ordered(p0, p1));
            REQUIRE (e::axiom_Totally_ordered(p1, p2));

            e::polynomial<int> p3{5, 2, 0, 3, 0};
            REQUIRE (p2 == p3);
            REQUIRE (p2 <= p3);
            REQUIRE (p2 >= p3);
        }

        SECTION ("Degree")
        {
            e::polynomial<int> p3{6, 4, 0, 3, 0, 0};

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
            e::polynomial<int> p3{6, 4, 0, 3};
            REQUIRE (p1 + p2 == p3);
            REQUIRE (axiom_Additive_group(p1, p2, p3));
        }

        SECTION ("Subtraction")
        {
            e::polynomial<int> p3{-4, 0, 0, -3};
            REQUIRE (p1 - p2 == p3);
        }

        SECTION ("Multiplication")
        {
            e::polynomial<int> p3{5, 12, 4, 3, 6};
            REQUIRE (p1 * p2 == p3);
            REQUIRE (p2 * p1 == p3);
            REQUIRE (axiom_Multiplicative_monoid(p1, p2, p3));
            REQUIRE (axiom_Ring(p0, p1, p2));
        }

        SECTION ("Scalar multiplication")
        {
            e::polynomial<int> p3{2, 4};
            REQUIRE (p1 * 2 == p3);

            // Commutative
            REQUIRE (p1 * 2 == 2 * p1);
        }

        SECTION ("Subderivatives")
        {
            e::polynomial<int> p3{8, -5, 0, 4, -1};
            e::polynomial<int> p4{-5, 0, 12, -4};
            e::polynomial<int> p5{0, 12, -6};
            e::polynomial<int> p6{4, -4};
            e::polynomial<int> p7{-1};

            REQUIRE (e::subderivative(p3, 0) == p3);
            REQUIRE (e::subderivative(p3, 1) == p4);
            REQUIRE (e::subderivative(p3, 2) == p5);
            REQUIRE (e::subderivative(p3, 3) == p6);
            REQUIRE (e::subderivative(p3, 4) == p7);
        }
    }
}
