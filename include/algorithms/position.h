#pragma once

#include "concepts/position.h"
#include "type_functions/position.h"

namespace elements {

template <typename T>
constexpr auto
pointer_to(T& x) -> pointer_type<T>
{
    return &x;
}

template <Semiregular T>
constexpr auto
load(T const& x) -> T const&
{
    return x;
}

template <Semiregular T>
constexpr auto
load(pointer_type<T> x) -> T const&
{
    return *x;
}

template <Semiregular T>
constexpr void
store(T& x, value_type<T> const& v)
{
    x = v;
}

template <Semiregular T>
constexpr void
store(pointer_type<T>& x, value_type<T> const& v)
{
    *x = v;
}

template <Semiregular T>
constexpr auto
at(T& x) -> T&
{
    return x;
}

template <Semiregular T>
constexpr auto
at(pointer_type<T> x) -> T&
{
    return *x;
}

}
