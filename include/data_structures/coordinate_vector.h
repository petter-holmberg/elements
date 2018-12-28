#pragma once

#include "array_k.h"
#include "algorithms/algebra.h"
#include "algorithms/fill.h"
#include "algorithms/map.h"

namespace elements {

template <
    typename S, int32_t k,
    typename E = array_k<S, k>>
requires
    Field<S> and
    Sequence<E>
struct coordinate_vector
{
    E elements;

    constexpr
    coordinate_vector()
        : elements{}
    {
        fill(first(elements), last(elements), Zero<S>);
    }

    constexpr
    coordinate_vector(std::initializer_list<S> x) : elements{x} {}

    constexpr auto
    operator[](int32_t i) const -> S const&
    {
        return elements[i];
    }

    constexpr auto
    operator[](int32_t i) -> S&
    {
        return elements[i];
    }
};

template <typename S, int32_t k, typename E>
requires Field<S>
struct underlying_type_t<coordinate_vector<S, k, E>>
{
    using type = coordinate_vector<Underlying_type<S>, k, Underlying_type<E>>;
};

template <typename S, int32_t k, typename E>
requires Field<S>
struct value_type_t<coordinate_vector<S, k, E>>
{
    using type = S;
};

template <typename S, int32_t k, typename E>
requires Field<S>
struct position_type_t<coordinate_vector<S, k, E>>
{
    using type = Position_type<E>;
};

template <typename S, int32_t k, typename E>
requires Field<S>
struct position_type_t<coordinate_vector<S, k, E> const>
{
    using type = Position_type<E const>;
};

template <typename S, int32_t k, typename E>
requires Field<S>
struct size_type_t<coordinate_vector<S, k, E>>
{
    using type = Size_type<E>;
    static constexpr auto value = k;
};

template <typename S, int32_t k, Regular E>
constexpr auto
operator==(coordinate_vector<S, k, E> const& x, coordinate_vector<S, k, E> const& y) -> bool
{
    return x.elements == y.elements;
}

template <typename S, int32_t k, Default_totally_ordered E>
constexpr auto
operator<(coordinate_vector<S, k, E> const& x, coordinate_vector<S, k, E> const& y) -> bool
{
    return x.elements < y.elements;
}

template <typename S, int32_t k, Range E>
constexpr auto
first(coordinate_vector<S, k, E> const& x) -> Position_type<E const>
{
    return first(x.elements);
}

template <typename S, int32_t k, Range E>
constexpr auto
first(coordinate_vector<S, k, E>& x) -> Position_type<E>
{
    return first(x.elements);
}

template <typename S, int32_t k, Range E>
constexpr auto
limit(coordinate_vector<S, k, E> const& x) -> Position_type<E const>
{
    return limit(x.elements);
}

template <typename S, int32_t k, Range E>
constexpr auto
limit(coordinate_vector<S, k, E>& x) -> Position_type<E>
{
    return limit(x.elements);
}

template <typename S, int32_t k, Range E>
constexpr auto
last(coordinate_vector<S, k, E> const& x) -> Position_type<E const>
{
    return last(x.elements);
}

template <typename S, int32_t k, Range E>
constexpr auto
last(coordinate_vector<S, k, E>& x) -> Position_type<E>
{
    return last(x.elements);
}

template <typename S, int32_t k, Range E>
constexpr auto
is_empty(coordinate_vector<S, k, E> const&) -> bool
{
    return k == 0;
}

template <typename S, int32_t k, Range E>
constexpr auto
size(coordinate_vector<S, k, E> const&) -> Size_type<coordinate_vector<S, k, E>>
{
    return k;
}

template <typename S, int32_t k, Range E>
requires Field<S>
constexpr auto
operator+(
    coordinate_vector<S, k, E> const& x, coordinate_vector<S, k, E> const& y) ->
    coordinate_vector<S, k, E>
{
    coordinate_vector<S, k, E> z;
    map(first(x), limit(x), first(y), first(z), sum<S>{});
    return z;
}

template <typename S, int32_t k, Range E>
requires Field<S>
constexpr auto
operator-(coordinate_vector<S, k, E> const& x) -> coordinate_vector<S, k, E>
{
    coordinate_vector<S, k, E> y;
    map(first(x), limit(x), first(y), negative<S>{});
    return y;
}

template <typename S, int32_t k, Range E>
requires Field<S>
constexpr auto
operator-(
    coordinate_vector<S, k, E> const& x, coordinate_vector<S, k, E> const& y) ->
    coordinate_vector<S, k, E>
{
    coordinate_vector<S, k, E> z;
    map(first(x), limit(x), first(y), first(z), difference<S>{});
    return z;
}

template <typename S, int32_t k, Range E>
requires Field<S>
constexpr auto
operator*(coordinate_vector<S, k, E> const& x, S const& n) -> coordinate_vector<S, k, E>
{
    coordinate_vector<S, k, E> z;
    map(first(x), limit(x), first(z), [&n](S const& x_){ return x_ * n; });
    return z;
}

template <typename S, int32_t k, Range E>
requires Field<S>
constexpr auto
operator*(S const& n, coordinate_vector<S, k, E> const& x) -> coordinate_vector<S, k, E>
{
    return x * n;
}

template <typename S, int32_t k, Range E>
requires Field<S>
constexpr auto
operator/(coordinate_vector<S, k, E> const& x, S const& n) -> coordinate_vector<S, k, E>
{
    coordinate_vector<S, k, E> z;
    map(first(x), limit(x), first(z), [&n](S const& x_){ return x_ / n; });
    return z;
}

}
