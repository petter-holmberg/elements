#pragma once

#include "concepts/integer.h"

namespace elements {

template <Binary_integer I>
constexpr void
increment(I& x)
{
    x = x + One<I>;
}

template <Binary_integer I>
constexpr void
decrement(I& x)
{
    x = x - One<I>;
}

template <Binary_integer I>
constexpr auto
successor(I x) -> I
{
    increment(x);
    return x;
}

template <Binary_integer I>
constexpr auto
predecessor(I x) -> I
{
    decrement(x);
    return x;
}

template <Binary_integer I>
constexpr auto
half(I const& a) -> I
{
    return a >> I{1};
}

template <Binary_integer I>
constexpr auto
twice(I const& a) -> I
{
    return a + a;
}

template <Binary_integer I>
constexpr auto
binary_scale_up(I const& a, I const& k) -> I
{
    return a << k;
}

template <Binary_integer I>
constexpr auto
binary_scale_down(I const& a, I const& k) -> I
{
    return a >> k;
}

template <Binary_integer I>
constexpr auto
is_positive(I const& a) -> bool
{
    return I{0} < a;
}

template <Binary_integer I>
constexpr auto
is_negative(I const& a) -> bool
{
    return a < I{0};
}

template <Binary_integer I>
constexpr auto
is_zero(I const& a) -> bool
{
    return a == I{0};
}

template <Binary_integer I>
constexpr auto
is_one(I const& a) -> bool
{
    return a == I{1};
}

template <Binary_integer I>
constexpr auto
is_even(I const& a) -> bool
{
    return (a bitand I{1}) == I{0};
}

template <Binary_integer I>
constexpr auto
is_odd(I const& a) -> bool
{
    return (a bitand I{1}) != I{0};
}

}
