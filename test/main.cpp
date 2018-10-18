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

int fn(int x, double y) { return x + static_cast<int>(y); }

struct fn_struct
{
    int fn(int x, double y) { return x + static_cast<int>(y); }
    int cfn(int x, double y) const { return x + static_cast<int>(y); }
    int mo;
};

struct fo_struct
{
    int operator()(int x, double y) { return x + static_cast<int>(y); }
};

SCENARIO ("Using invokable types", "[invokable]")
{
    SECTION ("free function")
    {
        static_assert(e::Same<e::domain<decltype(fn)>, int>);
        static_assert(e::Same<e::codomain<decltype(fn)>, int>);
        static_assert(e::arity<decltype(fn)> == 2);
        static_assert(e::Same<e::input_type<decltype(fn), 0>, int>);
        static_assert(e::Same<e::input_type<decltype(fn), 1>, double>);
    }

    SECTION ("function pointer")
    {
        static_assert(e::Same<e::domain<decltype(&fn)>, int>);
        static_assert(e::Same<e::codomain<decltype(&fn)>, int>);
        static_assert(e::arity<decltype(&fn)> == 2);
        static_assert(e::Same<e::input_type<decltype(&fn), 0>, int>);
        static_assert(e::Same<e::input_type<decltype(&fn), 1>, double>);
    }

    SECTION ("std::function")
    {
        static_assert(e::Same<e::domain<std::function<int(int, double)>>, int>);
        static_assert(e::Same<e::codomain<std::function<int(int, double)>>, int>);
        static_assert(e::arity<std::function<int(int, double)>> == 2);
        static_assert(e::Same<e::input_type<std::function<int(int, double)>, 0>, int>);
        static_assert(e::Same<e::input_type<std::function<int(int, double)>, 1>, double>);
    }

    SECTION ("member function pointer")
    {
        static_assert(e::Same<e::domain<decltype(&fn_struct::fn)>, int>);
        static_assert(e::Same<e::codomain<decltype(&fn_struct::fn)>, int>);
        static_assert(e::arity<decltype(&fn_struct::fn)> == 3);
        static_assert(e::Same<e::input_type<decltype(&fn_struct::fn), 1>, int>);
        static_assert(e::Same<e::input_type<decltype(&fn_struct::fn), 2>, double>);
    }

    SECTION ("const member function pointer")
    {
        static_assert(e::Same<e::domain<decltype(&fn_struct::cfn)>, int>);
        static_assert(e::Same<e::codomain<decltype(&fn_struct::cfn)>, int>);
        static_assert(e::arity<decltype(&fn_struct::cfn)> == 3);
        static_assert(e::Same<e::input_type<decltype(&fn_struct::cfn), 1>, int>);
        static_assert(e::Same<e::input_type<decltype(&fn_struct::cfn), 2>, double>);
    }

    SECTION ("member object pointer")
    {
        static_assert(e::Same<e::domain<decltype(&fn_struct::mo)>, fn_struct>);
        static_assert(e::Same<e::codomain<decltype(&fn_struct::mo)>, int>);
        static_assert(e::arity<decltype(&fn_struct::mo)> == 1);
        static_assert(e::Same<e::input_type<decltype(&fn_struct::mo), 0>, fn_struct&>);
    }

    SECTION ("function object")
    {
        fo_struct fo;

        static_assert(e::Same<e::domain<decltype(fo)>, int>);
        static_assert(e::Same<e::codomain<decltype(fo)>, int>);
        static_assert(e::arity<decltype(fo)> == 2);
        static_assert(e::Same<e::input_type<decltype(fo), 0>, int>);
        static_assert(e::Same<e::input_type<decltype(fo), 1>, double>);
    }

    SECTION ("lambda")
    {
        auto l = [](int x, double y){ return x + static_cast<int>(y); };

        static_assert(e::Same<e::domain<decltype(l)>, int>);
        static_assert(e::Same<e::codomain<decltype(l)>, int>);
        static_assert(e::arity<decltype(l)> == 2);
        static_assert(e::Same<e::input_type<decltype(l), 0>, int>);
        static_assert(e::Same<e::input_type<decltype(l), 1>, double>);
    }

    SECTION ("reference")
    {
        auto& fn_ref = fn;

        static_assert(e::Same<e::domain<decltype(fn_ref)>, int>);
        static_assert(e::Same<e::codomain<decltype(fn_ref)>, int>);
        static_assert(e::arity<decltype(fn_ref)> == 2);
        static_assert(e::Same<e::input_type<decltype(fn_ref), 0>, int>);
        static_assert(e::Same<e::input_type<decltype(fn_ref), 1>, double>);
    }
}
