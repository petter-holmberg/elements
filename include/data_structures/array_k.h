#pragma once

#include <cstddef>
#include <initializer_list>
#include <limits>

#include "concepts/invocable.h"
#include "algorithms/copy.h"
#include "algorithms/lexicographical.h"
#include "algorithms/map.h"
#include "algorithms/position.h"

namespace elements {

template <Semiregular T, std::ptrdiff_t k>
requires 0 < k and k <= std::numeric_limits<std::ptrdiff_t>::max() / sizeof(T)
struct array_k
{
    T data[static_cast<std::size_t>(k)];

    constexpr
    array_k()
        : data{}
    {}

    constexpr
    array_k(std::initializer_list<T> x)
        //[[expects: x.size() == k]]
        : data{}
    {
        copy(std::begin(x), std::end(x), data);
    }

    constexpr auto
    operator[](std::ptrdiff_t i) const -> T const&
    {
        return data[i];
    }

    constexpr auto
    operator[](std::ptrdiff_t i) -> T&
    {
        return data[i];
    }

    template <Unary_function Fun>
    requires
        Same<Decay<T>, Decay<Domain<Fun>>> and
        Same<Decay<T>, Decay<Codomain<Fun>>>
    constexpr auto
    map(Fun fun) -> array_k<T, k>&
    {
        using elements::map;
        map(first(*this), limit(*this), first(*this), fun);
        return *this;
    }

    template <Unary_function Fun>
    requires Same<Decay<T>, Decay<Domain<Fun>>>
    constexpr auto
    map(Fun fun) -> array_k<Decay<Codomain<Fun>>, k>
    {
        using elements::map;
        array_k<Decay<Codomain<Fun>>, k> x;
        map(first(*this), limit(*this), first(x), fun);
        return x;
    }

    template <Unary_function Fun>
    requires Same<Decay<T>, Decay<Domain<Fun>>>
    constexpr auto
    bind(Fun fun) -> array_k<Value_type<Decay<Codomain<Fun>>>, k * Size<Decay<Codomain<Fun>>>>
    {
        array_k<Value_type<Decay<Codomain<Fun>>>, k * Size<Decay<Codomain<Fun>>>> x;
        auto src = first(*this);
        auto lim = limit(*this);
        auto dst = first(x);
        while (precedes(src, lim)) {
            auto y = fun(load(src));
            dst = copy(first(y), limit(y), dst);
            increment(src);
        }
        return x;
    }
};

template <Semiregular T, std::ptrdiff_t k>
requires 0 < k and k <= std::numeric_limits<std::ptrdiff_t>::max() / sizeof(T)
struct underlying_type_t<array_k<T, k>>
{
    using type = array_k<Underlying_type<T>, k>;
};

template <Semiregular T, std::ptrdiff_t k>
struct value_type_t<array_k<T, k>>
{
    using type = T;
};

template <Semiregular T, std::ptrdiff_t k>
struct position_type_t<array_k<T, k>>
{
    using type = Pointer_type<T>;
};

template <Semiregular T, std::ptrdiff_t k>
struct position_type_t<array_k<T, k> const>
{
    using type = Pointer_type<T const>;
};

template <Semiregular T, std::ptrdiff_t k>
struct size_type_t<array_k<T, k>>
{
    using type = Distance_type<Pointer_type<T>>;
    static constexpr auto value = k;
};

template <Regular T, std::ptrdiff_t k>
constexpr auto
operator==(array_k<T, k> const& x, array_k<T, k> const& y) -> bool
{
    return lexicographical_equal(first(x), limit(x), first(y), limit(y));
}

template <Default_totally_ordered T, std::ptrdiff_t k>
constexpr auto
operator<(array_k<T, k> const& x, array_k<T, k> const& y) -> bool
{
    return lexicographical_less(first(x), limit(x), first(y), limit(y));
}

template <typename T, std::ptrdiff_t k>
constexpr auto
first(array_k<T, k> const& x) -> Pointer_type<T const>
{
    return pointer_to(x.data[0]);
}

template <typename T, std::ptrdiff_t k>
constexpr auto
first(array_k<T, k>& x) -> Pointer_type<T>
{
    return pointer_to(x.data[0]);
}

template <typename T, std::ptrdiff_t k>
constexpr auto
limit(array_k<T, k> const& x) -> Pointer_type<T const>
{
    return first(x) + k;
}

template <typename T, std::ptrdiff_t k>
constexpr auto
limit(array_k<T, k>& x) -> Pointer_type<T>
{
    return first(x) + k;
}

template <typename T, std::ptrdiff_t k>
constexpr auto
last(array_k<T, k>& x) -> Pointer_type<T>
{
    return limit(x) - 1;
}

template <typename T, std::ptrdiff_t k>
constexpr auto
is_empty(array_k<T, k> const&) -> bool
{
    return false;
}

template <typename T, std::ptrdiff_t k>
constexpr auto
size(array_k<T, k> const&) -> Size_type<array_k<T, k>>
{
    return k;
}

}
