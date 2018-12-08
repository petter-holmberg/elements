#pragma once

#include "concepts/regular.h"
#include "type_functions/position.h"

namespace elements {

// Access

template <typename T>
constexpr auto
pointer_to(T& x) -> Pointer_type<T>
{
    return &x;
}

template <typename T>
requires Semiregular<Decay<T>>
constexpr auto
load(T const& x) -> T const&
{
    return x;
}

template <typename T>
requires Semiregular<Decay<T>>
constexpr auto
load(Pointer_type<T> x) -> T const&
{
    return *x;
}

template <Semiregular T>
constexpr void
store(T& x, Value_type<T> const& v)
{
    x = v;
}

template <Semiregular T>
constexpr void
store(Pointer_type<T> x, Value_type<T> const& v)
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
at(Pointer_type<T> x) -> T&
{
    return *x;
}

// Linear traversal

template <typename T>
requires Semiregular<Decay<T>>
constexpr void
increment(Pointer_type<T>& x)
{
    x = x + Distance_type<Pointer_type<T>>{1};
}

template <typename T>
requires Semiregular<Decay<T>>
constexpr void
decrement(Pointer_type<T>& x)
{
    x = x - Distance_type<Pointer_type<T>>{1};
}

template <typename T>
requires Semiregular<Decay<T>>
constexpr auto
successor(Pointer_type<T> x) -> Pointer_type<T>
{
    increment(x);
    return x;
}

template <typename T>
requires Semiregular<Decay<T>>
constexpr auto
predecessor(Pointer_type<T> x) -> Pointer_type<T>
{
    decrement(x);
    return x;
}

}
