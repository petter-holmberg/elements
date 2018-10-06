#pragma once

#include "concepts/regular.h"
#include "type_functions/regular.h"

namespace elements {

template <Equality_comparable T>
struct equal
{
    constexpr auto
    operator()(T const& x, T const& y) -> bool
    {
        return x == y;
    }
};

template <Equality_comparable T>
struct less
{
    constexpr auto
    operator()(T const& x, T const& y) -> bool
    {
        return x < y;
    }
};

template <Semiregular T>
constexpr auto
operator!=(T const& x, T const& y) -> bool
{
    return !(x == y);
}

template <Regular T>
constexpr auto
operator>=(T const& x, T const& y) -> bool
{
    return !(x < y);
}

template <Regular T>
constexpr auto
operator>(T const& x, T const& y) -> bool
{
    return y < x;
}

template <Regular T>
constexpr auto
operator<=(T const& x, T const& y) -> bool
{
    return !(y < x);
}

template <typename T>
requires Semiregular<remove_cv<T>>
constexpr auto
underlying_ref(T& x) -> underlying_type<T>&
{
    return reinterpret_cast<underlying_type<T>&>(x);
}

}
