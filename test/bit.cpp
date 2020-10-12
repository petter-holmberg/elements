#include "catch.hpp"

#include "bit.h"

namespace e = elements;

SCENARIO ("Using bits", "[bit]")
{
    e::bit b0{0};
    e::bit b1{1};

    SECTION ("Algebra")
    {
        REQUIRE (e::axiom_Totally_ordered(b0, b1));

        SECTION ("Addition and mulitplication")
        {
            REQUIRE (b0 + b0 == b0);
            REQUIRE (b0 + b1 == b1);
            REQUIRE (b1 + b0 == b1);
            REQUIRE (b1 + b1 == b0);

            REQUIRE (b0 * b0 == b0);
            REQUIRE (b0 * b1 == b0);
            REQUIRE (b1 * b0 == b0);
            REQUIRE (b1 * b1 == b1);

            REQUIRE (e::axiom_Commutative_semiring(b0, b1, b0));
        }
    }

    SECTION ("Logical operations")
    {
        REQUIRE (e::contradiction(b0, b0) == b0);
        REQUIRE (e::contradiction(b0, b1) == b0);
        REQUIRE (e::contradiction(b1, b0) == b0);
        REQUIRE (e::contradiction(b1, b1) == b0);

        REQUIRE (e::conjunction(b0, b0) == b0);
        REQUIRE (e::conjunction(b0, b1) == b0);
        REQUIRE (e::conjunction(b1, b0) == b0);
        REQUIRE (e::conjunction(b1, b1) == b1);

        REQUIRE (e::nonimplication(b0, b0) == b0);
        REQUIRE (e::nonimplication(b0, b1) == b0);
        REQUIRE (e::nonimplication(b1, b0) == b1);
        REQUIRE (e::nonimplication(b1, b1) == b0);

        REQUIRE (e::projection_left(b0, b0) == b0);
        REQUIRE (e::projection_left(b0, b1) == b0);
        REQUIRE (e::projection_left(b1, b0) == b1);
        REQUIRE (e::projection_left(b1, b1) == b1);

        REQUIRE (e::converse_nonimplication(b0, b0) == b0);
        REQUIRE (e::converse_nonimplication(b0, b1) == b1);
        REQUIRE (e::converse_nonimplication(b1, b0) == b0);
        REQUIRE (e::converse_nonimplication(b1, b1) == b0);

        REQUIRE (e::projection_right(b0, b0) == b0);
        REQUIRE (e::projection_right(b0, b1) == b1);
        REQUIRE (e::projection_right(b1, b0) == b0);
        REQUIRE (e::projection_right(b1, b1) == b1);

        REQUIRE (e::nonequivalence(b0, b0) == b0);
        REQUIRE (e::nonequivalence(b0, b1) == b1);
        REQUIRE (e::nonequivalence(b1, b0) == b1);
        REQUIRE (e::nonequivalence(b1, b1) == b0);

        REQUIRE (e::disjunction(b0, b0) == b0);
        REQUIRE (e::disjunction(b0, b1) == b1);
        REQUIRE (e::disjunction(b1, b0) == b1);
        REQUIRE (e::disjunction(b1, b1) == b1);

        REQUIRE (e::nondisjunction(b0, b0) == b1);
        REQUIRE (e::nondisjunction(b0, b1) == b0);
        REQUIRE (e::nondisjunction(b1, b0) == b0);
        REQUIRE (e::nondisjunction(b1, b1) == b0);

        REQUIRE (e::equivalence(b0, b0) == b1);
        REQUIRE (e::equivalence(b0, b1) == b0);
        REQUIRE (e::equivalence(b1, b0) == b0);
        REQUIRE (e::equivalence(b1, b1) == b1);

        REQUIRE (e::nonprojection_right(b0, b0) == b1);
        REQUIRE (e::nonprojection_right(b0, b1) == b0);
        REQUIRE (e::nonprojection_right(b1, b0) == b1);
        REQUIRE (e::nonprojection_right(b1, b1) == b0);

        REQUIRE (e::converse_implication(b0, b0) == b1);
        REQUIRE (e::converse_implication(b0, b1) == b0);
        REQUIRE (e::converse_implication(b1, b0) == b1);
        REQUIRE (e::converse_implication(b1, b1) == b1);

        REQUIRE (e::nonprojection_left(b0, b0) == b1);
        REQUIRE (e::nonprojection_left(b0, b1) == b1);
        REQUIRE (e::nonprojection_left(b1, b0) == b0);
        REQUIRE (e::nonprojection_left(b1, b1) == b0);

        REQUIRE (e::implication(b0, b0) == b1);
        REQUIRE (e::implication(b0, b1) == b1);
        REQUIRE (e::implication(b1, b0) == b0);
        REQUIRE (e::implication(b1, b1) == b1);

        REQUIRE (e::nonconjunction(b0, b0) == b1);
        REQUIRE (e::nonconjunction(b0, b1) == b1);
        REQUIRE (e::nonconjunction(b1, b0) == b1);
        REQUIRE (e::nonconjunction(b1, b1) == b0);

        REQUIRE (e::tautology(b0, b0) == b1);
        REQUIRE (e::tautology(b0, b1) == b1);
        REQUIRE (e::tautology(b1, b0) == b1);
        REQUIRE (e::tautology(b1, b1) == b1);
    }
}
