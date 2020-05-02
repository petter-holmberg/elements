#pragma once

#include "algebra.h"

namespace elements {

template <typename T>
concept Integral = exposition_only::Is_integral<T>;

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
half(I const& a) -> I
{
    return a >> I{1};
}

template <Integral I>
constexpr auto
twice(I const& a) -> I
{
    return a + a;
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

}
