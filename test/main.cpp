#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <chrono>
#include <complex>
#include <cstddef>

#include "elements.h"

namespace e = elements;

SCENARIO ("Using built-in and library algebraic types", "[algebra]")
{
    SECTION ("Semigroups")
    {
        static_assert(e::Semigroup<int, e::sum<int>>);
        static_assert(e::Semigroup<int, e::product<int>>);

        static_assert(e::Additive_semigroup<int>);

        static_assert(e::Multiplicative_semigroup<int>);
    }

    SECTION ("Monoids")
    {
        static_assert(e::Monoid<int, e::sum<int>>);
        static_assert(e::Monoid<int, e::product<int>>);

        static_assert(e::Additive_monoid<int>);

        static_assert(e::Multiplicative_monoid<int>);
    }

    SECTION ("Groups")
    {
        static_assert(e::Group<int, e::sum<int>>);
        static_assert(e::Group<int, e::product<int>>);

        static_assert(e::Additive_group<int>);

        static_assert(e::Multiplicative_group<int>);
    }

    SECTION ("Semirings")
    {
        static_assert(e::Semiring<int, e::sum<int>, e::product<int>>);

        static_assert(e::Commutative_semiring<int, e::sum<int>, e::product<int>>);

        static_assert(e::Integral_semiring<int>);
    }

    SECTION ("Rings")
    {
        static_assert(e::Ring<int, e::sum<int>, e::product<int>>);

        static_assert(e::Commutative_ring<int, e::sum<int>, e::product<int>>);

        static_assert(e::Integral_ring<int>);
    }

    SECTION ("Integral domains")
    {
        static_assert(e::Integral_domain<int, e::sum<int>, e::product<int>>);
    }

    SECTION ("Fields")
    {
        static_assert(e::Field<double, e::sum<double>, e::product<double>>);
        static_assert(
            e::Field<
                std::complex<double>,
                e::sum<std::complex<double>>,
                e::product<std::complex<double>>>);
    }

    SECTION ("Semimodules")
    {
        static_assert(
            e::Left_semimodule<
                std::ptrdiff_t,
                e::sum<std::ptrdiff_t>,
                int,
                e::sum<int>,
                e::product<int>>);

        static_assert(
            e::Right_semimodule<
                std::ptrdiff_t,
                e::sum<std::ptrdiff_t>,
                int,
                e::sum<int>,
                e::product<int>>);

        static_assert(
            e::Semimodule<
                std::ptrdiff_t,
                e::sum<std::ptrdiff_t>,
                int,
                e::sum<int>,
                e::product<int>>);
    }

    SECTION ("Modules")
    {
        static_assert(
            e::Left_module<
                std::ptrdiff_t,
                e::sum<std::ptrdiff_t>,
                int,
                e::sum<int>,
                e::product<int>>);

        static_assert(
            e::Right_module<
                std::ptrdiff_t,
                e::sum<std::ptrdiff_t>,
                int,
                e::sum<int>,
                e::product<int>>);

        static_assert(
            e::Module<
                std::ptrdiff_t,
                e::sum<std::ptrdiff_t>,
                int,
                e::sum<int>,
                e::product<int>>);
    }

    SECTION ("Vector spaces")
    {
        static_assert(e::Vector_space<std::ptrdiff_t, int>);
    }

    SECTION ("Affine spaces")
    {
        static_assert(e::Affine_space<int*, std::ptrdiff_t, int>);

        static_assert(
            e::Affine_space<
                std::chrono::system_clock::time_point,
                std::chrono::system_clock::duration,
                int>);
    }
}
