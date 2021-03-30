#pragma once

#include "algebra.h"

namespace elements {

template <Integral I>
constexpr void
increment(I& x)
{
    ++x;
}

template <Integral I>
constexpr void
decrement(I& x)
{
    --x;
}

template <Integral I>
constexpr auto
twice(I const& a) -> I
{
    return a + a;
}

template <Integral I>
constexpr auto
half(I const& a) -> I
{
    return a >> I{1};
}

template <Integral I>
constexpr auto
binary_scale_up(I const& a, I const& k) -> I
{
    return a << k;
}

template <Integral I>
constexpr auto
binary_scale_down(I const& a, I const& k) -> I
{
    return a >> k;
}

template <Integral I>
constexpr auto
is_positive(I const& a) -> bool
{
    return I{0} < a;
}

template <Integral I>
constexpr auto
is_negative(I const& a) -> bool
{
    return a < I{0};
}

template <Integral I>
constexpr auto
is_zero(I const& a) -> bool
{
    return a == Zero<I>;
}

template <Integral I>
constexpr auto
is_one(I const& a) -> bool
{
    return a == I{1};
}

template <Integral I>
constexpr auto
is_even(I const& a) -> bool
{
    return (a bitand I{1}) == I{0};
}

template <Integral I>
constexpr auto
is_odd(I const& a) -> bool
{
    return (a bitand I{1}) != I{0};
}

template <Integral I>
constexpr auto
precedes(I const& i0, I const& i1) -> bool
{
    return i0 < i1;
}

template <typename, typename...>
struct distance_type_t;

template <typename T, typename... Us>
using Distance_type = typename distance_type_t<T, Us...>::type;

template <Integral I>
struct distance_type_t<I>
{
    using type = Unsigned_type<I>;
};

template <typename I>
concept Integer =
    requires (I a) {
        { increment(a) } -> Same_as<void>;
        { decrement(a) } -> Same_as<void>;
        { twice(a) } -> Same_as<I>;
        { half(a) } -> Same_as<I>;
        { binary_scale_up(a, a) } -> Same_as<I>;
        { binary_scale_down(a, a) } -> Same_as<I>;
        { is_positive(a) } -> Boolean_testable;
        { is_negative(a) } -> Boolean_testable;
        { is_zero(a) } -> Boolean_testable;
        { is_one(a) } -> Boolean_testable;
        { is_even(a) } -> Boolean_testable;
        { is_odd(a) } -> Boolean_testable;
    };

template <Integer N>
constexpr auto
count_down(N& n) -> bool
{
    if (is_zero(n)) return false;
    decrement(n);
    return true;
}

}
