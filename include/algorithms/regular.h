#pragma once

#include <cstdlib>
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

template <typename T>
requires Default_constructible<Remove_const<T>>
constexpr void
construct(T& raw)
    //[[expects axiom: raw_memory(raw)]]
    //[[ensures axiom: partially_formed(raw)]]
{
    new (&raw) T();
}

template <typename T, typename U>
requires Default_constructible<Remove_const<T>>
constexpr void
construct(T& raw, U&& initializer)
    //[[expects axiom: raw_memory(raw)]]
    //[[ensures axiom: raw == initializer]]
{
    new (&raw) T(fw<U>(initializer));
}

template <typename T>
requires Destructible<Remove_const<T>>
constexpr void
destroy(T& x)
    //[[expects axiom: partially_formed(x)]]
    //[[ensures axiom: raw_memory(x)]]
{
    x.~T();
}

template <typename T, typename E>
constexpr auto
allocate(E extra_bytes = Zero<E>) -> Pointer_type<T>
{
    return reinterpret_cast<Pointer_type<T>>(
        std::malloc(sizeof(T) + static_cast<std::size_t>(extra_bytes)));
}

template <typename T>
constexpr void
deallocate(Pointer_type<T> raw)
{
    std::free(reinterpret_cast<void*>(raw));
}

}
