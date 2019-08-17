#pragma once

#include "array_k.h"
#include "coordinate_vector.h"
#include "algorithms/algebra.h"
#include "algorithms/fill.h"
#include "algorithms/map.h"
#include "algorithms/regular.h"

namespace elements {

template <
    typename S,
    int32_t k,
    typename C = array_k<S, k>,
    typename V = coordinate_vector<S, k, C>>
requires
    Field<S> and
    Sequence<C> and
    Vector_space<V> and
    Sequence<V>
struct coordinate_point
{
    C coordinates;

    constexpr
    coordinate_point()
        : coordinates{}
    {
        fill(first(coordinates), limit(coordinates), Zero<Value_type<V>>);
    }

    constexpr
    coordinate_point(std::initializer_list<S> x) : coordinates{x} {}

    constexpr auto
    operator[](int32_t i) const -> S const&
    {
        return coordinates[i];
    }

    constexpr auto
    operator[](int32_t i) -> S&
    {
        return coordinates[i];
    }
};

template <typename S, int32_t k, typename C, typename V>
requires
    Field<S> and
    Sequence<C> and
    Vector_space<V> and
    Sequence<V>
struct value_type_t<coordinate_point<S, k, C, V>>
{
    using type = S;
};

template <typename S, int32_t k, typename C, typename V>
requires
    Field<S> and
    Sequence<C> and
    Vector_space<V> and
    Sequence<V>
struct difference_type_t<coordinate_point<S, k, C, V>>
{
    using type = V;
};

template <typename S, int32_t k, typename C, typename V>
requires
    Field<S> and
    Sequence<C> and
    Vector_space<V> and
    Sequence<V>
struct position_type_t<coordinate_point<S, k, C, V>>
{
    using type = Position_type<C>;
};

template <typename S, int32_t k, typename C, typename V>
requires
    Field<S> and
    Sequence<C> and
    Vector_space<V> and
    Sequence<V>
struct position_type_t<coordinate_point<S, k, C, V> const>
{
    using type = Position_type<C const>;
};

template <typename S, int32_t k, typename C, typename V>
requires
    Field<S> and
    Sequence<C> and
    Vector_space<V> and
    Sequence<V>
struct size_type_t<coordinate_point<S, k, C, V>>
{
    using type = Size_type<C>;
    static constexpr auto value = k;
};

template <typename S, int32_t k, typename C, Regular V>
constexpr auto
operator==(coordinate_point<S, k, C, V> const& x, coordinate_point<S, k, C, V> const& y) -> bool
{
    return x.coordinates == y.coordinates;
}

template <typename S, int32_t k, typename C, Default_totally_ordered V>
struct less<coordinate_point<S, k, C, V>>
{
    constexpr auto
    operator()(coordinate_point<S, k, C, V> const& x, coordinate_point<S, k, C, V> const& y) -> bool
    {
        return less<C>{}(x.coordinates, y.coordinates);
    }
};

template <typename S, int32_t k, Range C, typename V>
constexpr auto
first(coordinate_point<S, k, C, V> const& x) -> Position_type<V const>
{
    return first(x.coordinates);
}

template <typename S, int32_t k, Range C, typename V>
constexpr auto
first(coordinate_point<S, k, C, V>& x) -> Position_type<V>
{
    return first(x.coordinates);
}

template <typename S, int32_t k, Range C, typename V>
constexpr auto
limit(coordinate_point<S, k, C, V> const& x) -> Position_type<V const>
{
    return limit(x.coordinates);
}

template <typename S, int32_t k, Range C, typename V>
constexpr auto
limit(coordinate_point<S, k, C, V>& x) -> Position_type<V>
{
    return limit(x.coordinates);
}

template <typename S, int32_t k, Range C, typename V>
constexpr auto
is_empty(coordinate_point<S, k, C, V> const&) -> bool
{
    return k == 0;
}

template <typename S, int32_t k, Range C, typename V>
constexpr auto
size(coordinate_point<S, k, C, V> const&) -> Size_type<coordinate_point<S, k, C, V>>
{
    return k;
}

template <typename S, int32_t k, Range C, typename V>
requires
    Field<S> and
    Vector_space<V> and
    Sequence<V>
constexpr auto
operator-(coordinate_point<S, k, C, V> const& x, coordinate_point<S, k, C, V> const& y) -> V
{
    V v;
    map(first(x), limit(x), first(y), first(v), difference<S>{});
    return v;
}

template <typename S, int32_t k, Range C, typename V>
requires
    Field<S> and
    Vector_space<V> and
    Sequence<V>
constexpr auto
operator+(coordinate_point<S, k, C, V> const& x, V const& v) -> coordinate_point<S, k, C, V>
{
    coordinate_point<S, k, C, V> y;
    map(first(x), limit(x), first(v), first(y), [](S const& a, S const& b){ return a + b; });
    return y;
}

template <typename S, int32_t k, Range C, typename V>
requires
    Field<S> and
    Vector_space<V> and
    Sequence<V>
constexpr auto
operator+(V const& v, coordinate_point<S, k, C, V> const& x) -> coordinate_point<S, k, C, V>
{
    return x + v;
}

template <typename S, int32_t k, Range C, typename V>
requires
    Field<S> and
    Vector_space<V> and
    Sequence<V>
constexpr auto
operator-(coordinate_point<S, k, C, V> const& x, V const& v) -> coordinate_point<S, k, C, V>
{
    coordinate_point<S, k, C, V> y;
    map(first(x), limit(x), first(v), first(y), [](S const& a, S const& b){ return a - b; });
    return y;
}

}
