#pragma once

namespace elements {

template <typename T>
constexpr bool Is_integral = std::is_integral<T>::value;

template <typename I>
requires Is_integral<I>
constexpr void
increment(I& x)
{
    x = x + One<I>;
}

template <typename I>
requires Is_integral<I>
constexpr void
decrement(I& x)
{
    x = x - One<I>;
}

template <typename I>
requires Is_integral<I>
constexpr auto
successor(I x) -> I
{
    increment(x);
    return x;
}

template <typename I>
requires Is_integral<I>
constexpr auto
predecessor(I x) -> I
{
    decrement(x);
    return x;
}

template <typename I>
requires Is_integral<I>
constexpr auto
half(I const& a) -> I
{
    return a >> I{1};
}

template <typename I>
requires Is_integral<I>
constexpr auto
twice(I const& a) -> I
{
    return a + a;
}

template <typename I>
requires Is_integral<I>
constexpr auto
binary_scale_up_nonnegative(I const& a, I const& k) -> I
{
    return a << k;
}

template <typename I>
requires Is_integral<I>
constexpr auto
binary_scale_down_nonnegative(I const& a, I const& k) -> I
{
    return a >> k;
}

template <typename I>
requires Is_integral<I>
constexpr auto
is_positive(I const& a) -> bool
{
    return I{0} < a;
}

template <typename I>
requires Is_integral<I>
constexpr auto
is_negative(I const& a) -> bool
{
    return a < I{0};
}

template <typename I>
requires Is_integral<I>
constexpr auto
is_zero(I const& a) -> bool
{
    return a == I{0};
}

template <typename I>
requires Is_integral<I>
constexpr auto
is_one(I const& a) -> bool
{
    return a == I{1};
}

template <typename I>
requires Is_integral<I>
constexpr auto
is_even(I const& a) -> bool
{
    return (a bitand I{1}) == I{0};
}

template <typename I>
requires Is_integral<I>
constexpr auto
is_odd(I const& a) -> bool
{
    return (a bitand I{1}) != I{0};
}

}
