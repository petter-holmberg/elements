#pragma once

#include <utility>
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
struct equal_unary
{
    T const& x;

    constexpr
    equal_unary(T const& x_) : x{x_} {}

    constexpr auto
    operator()(T const& y) -> bool
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
requires Semiregular<Remove_cv<T>>
constexpr auto
underlying_ref(T& x) noexcept -> Underlying_type<T>&
{
    return reinterpret_cast<Underlying_type<T>&>(x);
}

template <typename T>
constexpr auto
fw(Remove_ref<T>& x) noexcept -> T&&
{
    return std::forward<T>(x);
}

template<typename T>
constexpr auto
fw(Remove_ref<T>&& x) noexcept -> T&&
{
    return std::forward<T>(x);
}

template <typename T>
constexpr auto
mv(T&& x) noexcept -> Remove_ref<T>&&
{
    return std::move(x);
}

}
