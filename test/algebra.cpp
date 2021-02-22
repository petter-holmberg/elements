#include "catch.hpp"

#include <complex>
#include <cstddef>

#include "algebra.h"
#include "cursor.h"

namespace e = elements;

SCENARIO ("Using built-in and library algebraic types", "[algebra]")
{
    SECTION ("Semigroups")
    {
        static_assert(e::Semigroup<int, e::add_op<int>>);
        static_assert(axiom_Semigroup(1, 2, 3, e::add));

        static_assert(e::Semigroup<int, e::mul_op<int>>);
        static_assert(axiom_Semigroup(2, 3, 4, e::multiply));

        static_assert(e::Additive_semigroup<int>);
        static_assert(e::axiom_Additive_semigroup(1, 2, 3));

        static_assert(e::Multiplicative_semigroup<int>);
        static_assert(e::axiom_Multiplicative_semigroup(2, 3, 4));
    }

    SECTION ("Monoids")
    {
        static_assert(e::Monoid<int, e::add_op<int>>);
        static_assert(axiom_Monoid(1, 2, 3, e::add));

        static_assert(e::Monoid<int, e::mul_op<int>>);
        static_assert(axiom_Monoid(2, 3, 4, e::multiply));

        static_assert(e::Additive_monoid<int>);
        static_assert(e::axiom_Additive_monoid(1, 2, 3));

        static_assert(e::Multiplicative_monoid<int>);
        static_assert(e::axiom_Multiplicative_monoid(2, 3, 4));
    }

    SECTION ("Groups")
    {
        static_assert(e::Group<int, e::add_op<int>>);
        static_assert(e::axiom_Group(1, 2, 3, e::add));

        static_assert(e::Additive_group<int>);
        static_assert(e::axiom_Additive_group(1, 2, 3));
    }

    SECTION ("Semirings")
    {
        static_assert(e::Semiring<int>);
        static_assert(e::axiom_Semiring(2, 3, 4));

        static_assert(e::Commutative_semiring<int>);
        static_assert(e::axiom_Commutative_semiring(2, 3, 4));
    }

    SECTION ("Rings")
    {
        static_assert(e::Ring<int>);
        static_assert(e::axiom_Ring(2, 3, 4));

        static_assert(e::Commutative_ring<int>);
        static_assert(e::axiom_Commutative_ring(2, 3, 4));
    }

    SECTION ("Integral domains")
    {
        static_assert(e::Integral_domain<int>);
        static_assert(e::axiom_Integral_domain(2, 3, 4));
    }

    SECTION ("Fields")
    {
        static_assert(e::Field<double>);
        static_assert(e::Field<std::complex<double>>);
    }

    SECTION ("Semimodules")
    {
        static_assert(e::Left_semimodule<std::ptrdiff_t>);

        static_assert(e::Right_semimodule<std::ptrdiff_t>);

        static_assert(e::Semimodule<std::ptrdiff_t>);
    }

    SECTION ("Modules")
    {
        static_assert(e::Left_module<std::ptrdiff_t>);

        static_assert(e::Right_module<std::ptrdiff_t>);

        static_assert(e::Module<std::ptrdiff_t>);
    }

    SECTION ("Vector spaces")
    {
        static_assert(e::Vector_space<std::ptrdiff_t>);
    }
}
