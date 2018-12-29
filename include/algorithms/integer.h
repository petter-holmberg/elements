#pragma once

#include <type_traits>

#include "regular.h"
#include "algebra.h"

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

}
