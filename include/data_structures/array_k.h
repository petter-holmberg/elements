#pragma once

#include <initializer_list>
#include <limits>

#include "algorithms/copy.h"
#include "algorithms/position.h"
#include "algorithms/lexicographical.h"

namespace elements {

template <typename T, std::ptrdiff_t k>
requires
    Semiregular<T> and
    (0 < k and k <= std::numeric_limits<std::ptrdiff_t>::max() / sizeof(T))
struct array_k
{
    T data[k];

    constexpr
    array_k() = default;

    constexpr
    array_k(std::initializer_list<T> x)
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
};

template <typename T, std::ptrdiff_t k>
requires
    Semiregular<T> and
    (0 < k and k <= std::numeric_limits<std::ptrdiff_t>::max() / sizeof(T))
struct underlying_type_t<array_k<T, k>>
{
    using type = array_k<Underlying_type<T>, k>;
};

template <typename T, std::ptrdiff_t k>
requires Semiregular<T>
struct value_type_t<array_k<T, k>>
{
    using type = T;
};

template <typename T, std::ptrdiff_t k>
requires Semiregular<T>
struct position_type_t<array_k<T, k>>
{
    using type = Pointer_type<T>;
};

template <typename T, std::ptrdiff_t k>
requires Semiregular<T>
struct position_type_t<array_k<T, k> const>
{
    using type = Pointer_type<T const>;
};

template <typename T, std::ptrdiff_t k>
requires Semiregular<T>
struct size_type_t<array_k<T, k>>
{
    using type = Distance_type<Pointer_type<T>>;
    static constexpr auto value = k;
};

template <typename T, std::ptrdiff_t k>
requires Regular<T>
constexpr auto
operator==(array_k<T, k> const& x, array_k<T, k> const& y) -> bool
{
    return lexicographical_equal(first(x), limit(x), first(y), limit(y));
}

template <typename T, std::ptrdiff_t k>
requires Default_totally_ordered<T>
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
last(array_k<T, k> const& x) -> Pointer_type<T const>
{
    return limit(x) - 1;
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
