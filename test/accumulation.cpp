#include "catch.hpp"

#include <complex>
#include <cstddef>

#include "accumulation.h"

namespace e = elements;

auto sum_accumulate = [](int& a, int const& b) { a = a + b; };

template <>
struct e::identity_element_t<int, decltype(sum_accumulate)>
{
    static int const value = 0;
};

template <>
struct e::inverse_accumulator<int, decltype(sum_accumulate)>
{
    constexpr void
    operator()(int& a) const
    {
        a = -a;
    }
};

auto product_accumulate = [](int& a, int const& b) { a = a * b; };

template <>
struct e::identity_element_t<int, decltype(product_accumulate)>
{
    static int const value = 1;
};

SCENARIO ("Calculating powers", "[integer]")
{

    SECTION ("Binary operations")
    {
        static_assert(e::power_left_associated(1, 1, e::add) == 1);
        static_assert(e::power_left_associated(1, 2, e::add) == 2);
        static_assert(e::power_left_associated(2, 1, e::add) == 2);
        static_assert(e::power_left_associated(2, 2, e::add) == 4);
        static_assert(e::power_right_associated(1, 1, e::add) == 1);
        static_assert(e::power_right_associated(1, 2, e::add) == 2);
        static_assert(e::power_right_associated(2, 1, e::add) == 2);
        static_assert(e::power_right_associated(2, 2, e::add) == 4);

        static_assert(e::power_left_associated(1, 1, sum_accumulate) == 1);
        static_assert(e::power_left_associated(1, 2, sum_accumulate) == 2);
        static_assert(e::power_left_associated(2, 1, sum_accumulate) == 2);
        static_assert(e::power_left_associated(2, 2, sum_accumulate) == 4);
        static_assert(e::power_right_associated(1, 1, sum_accumulate) == 1);
        static_assert(e::power_right_associated(1, 2, sum_accumulate) == 2);
        static_assert(e::power_right_associated(2, 1, sum_accumulate) == 2);
        static_assert(e::power_right_associated(2, 2, sum_accumulate) == 4);

        static_assert(e::power_left_associated(1, 1, e::multiply) == 1);
        static_assert(e::power_left_associated(1, 2, e::multiply) == 1);
        static_assert(e::power_left_associated(2, 1, e::multiply) == 2);
        static_assert(e::power_left_associated(2, 2, e::multiply) == 4);
        static_assert(e::power_right_associated(1, 1, e::multiply) == 1);
        static_assert(e::power_right_associated(1, 2, e::multiply) == 1);
        static_assert(e::power_right_associated(2, 1, e::multiply) == 2);
        static_assert(e::power_right_associated(2, 2, e::multiply) == 4);

        static_assert(e::power_left_associated(1, 1, product_accumulate) == 1);
        static_assert(e::power_left_associated(1, 2, product_accumulate) == 1);
        static_assert(e::power_left_associated(2, 1, product_accumulate) == 2);
        static_assert(e::power_left_associated(2, 2, product_accumulate) == 4);
        static_assert(e::power_right_associated(1, 1, product_accumulate) == 1);
        static_assert(e::power_right_associated(1, 2, product_accumulate) == 1);
        static_assert(e::power_right_associated(2, 1, product_accumulate) == 2);
        static_assert(e::power_right_associated(2, 2, product_accumulate) == 4);
    }

    SECTION ("Semigroups")
    {
        static_assert(e::power_semigroup(1, 1, e::add) == 1);
        static_assert(e::power_semigroup(1, 2, e::add) == 2);
        static_assert(e::power_semigroup(2, 1, e::add) == 2);
        static_assert(e::power_semigroup(2, 2, e::add) == 4);

        static_assert(e::power_semigroup(1, 1, sum_accumulate) == 1);
        static_assert(e::power_semigroup(1, 2, sum_accumulate) == 2);
        static_assert(e::power_semigroup(2, 1, sum_accumulate) == 2);
        static_assert(e::power_semigroup(2, 2, sum_accumulate) == 4);

        static_assert(e::power_semigroup(1, 1, e::multiply) == 1);
        static_assert(e::power_semigroup(1, 2, e::multiply) == 1);
        static_assert(e::power_semigroup(2, 1, e::multiply) == 2);
        static_assert(e::power_semigroup(2, 2, e::multiply) == 4);

        static_assert(e::power_semigroup(1, 1, product_accumulate) == 1);
        static_assert(e::power_semigroup(1, 2, product_accumulate) == 1);
        static_assert(e::power_semigroup(2, 1, product_accumulate) == 2);
        static_assert(e::power_semigroup(2, 2, product_accumulate) == 4);
    }

    SECTION ("Monoids")
    {
        static_assert(e::power_monoid(1, 0, e::add) == 0);
        static_assert(e::power_monoid(1, 1, e::add) == 1);
        static_assert(e::power_monoid(1, 2, e::add) == 2);
        static_assert(e::power_monoid(2, 1, e::add) == 2);
        static_assert(e::power_monoid(2, 2, e::add) == 4);

        static_assert(e::power_monoid(1, 0, sum_accumulate) == 0);
        static_assert(e::power_monoid(1, 1, sum_accumulate) == 1);
        static_assert(e::power_monoid(1, 2, sum_accumulate) == 2);
        static_assert(e::power_monoid(2, 1, sum_accumulate) == 2);
        static_assert(e::power_monoid(2, 2, sum_accumulate) == 4);

        static_assert(e::power_monoid(1, 0, e::multiply) == 1);
        static_assert(e::power_monoid(1, 1, e::multiply) == 1);
        static_assert(e::power_monoid(1, 2, e::multiply) == 1);
        static_assert(e::power_monoid(2, 1, e::multiply) == 2);
        static_assert(e::power_monoid(2, 2, e::multiply) == 4);

        static_assert(e::power_monoid(1, 0, product_accumulate) == 1);
        static_assert(e::power_monoid(1, 1, product_accumulate) == 1);
        static_assert(e::power_monoid(1, 2, product_accumulate) == 1);
        static_assert(e::power_monoid(2, 1, product_accumulate) == 2);
        static_assert(e::power_monoid(2, 2, product_accumulate) == 4);
    }

    SECTION ("Groups")
    {
        static_assert(e::power_group(2, -2, e::add) == -4);
        static_assert(e::power_group(2, -1, e::add) == -2);
        static_assert(e::power_group(1, -2, e::add) == -2);
        static_assert(e::power_group(1, -1, e::add) == -1);
        static_assert(e::power_group(1, 0, e::add) == 0);
        static_assert(e::power_group(1, 1, e::add) == 1);
        static_assert(e::power_group(1, 2, e::add) == 2);
        static_assert(e::power_group(2, 1, e::add) == 2);
        static_assert(e::power_group(2, 2, e::add) == 4);
        static_assert(e::power_group(2, -2, sum_accumulate) == -4);
        static_assert(e::power_group(2, -1, sum_accumulate) == -2);
        static_assert(e::power_group(1, -2, sum_accumulate) == -2);
        static_assert(e::power_group(1, -1, sum_accumulate) == -1);
        static_assert(e::power_group(1, 0, sum_accumulate) == 0);
        static_assert(e::power_group(1, 1, sum_accumulate) == 1);
        static_assert(e::power_group(1, 2, sum_accumulate) == 2);
        static_assert(e::power_group(2, 1, sum_accumulate) == 2);
        static_assert(e::power_group(2, 2, sum_accumulate) == 4);
    }
}
