#pragma once

#include "accumulation.h"
#include "vector_space.h"
#include "reduce.h"

namespace elements {

template <typename P, typename V = Difference_type<P>, typename S = Scalar_type<V>>
concept Affine_space =
    Regular<P> and
    Vector_space<V, S> and
    requires (P const& p, V const& v) {
        { p - p } -> Same_as<V>;
        { p + v } -> Same_as<P>;
        { v + p } -> Same_as<P>;
        { p - v } -> Same_as<P>;
    };

template <Affine_space P>
struct vector_type_t;

template <Affine_space P>
using Vector_type = typename vector_type_t<P>::type;

template <typename P, typename V = Difference_type<P>, typename S = Scalar_type<V>>
requires Affine_space<P>
constexpr auto
operator+=(P& p, V const& v) -> P&
{
    p = p + v;
    return p;
}

template <typename P, typename V = Difference_type<P>, typename S = Scalar_type<V>>
requires Affine_space<P>
constexpr auto
operator-=(P& p, V const& v) -> P&
{
    p = p - v;
    return p;
}

template <Semiring S, pointer_diff k, Sequence C = array_k<S, k>, Vector_space V = static_vector<S, k>>
struct point
{
    C coordinates;

    constexpr
    point()
        : coordinates{}
    {
        fill(first(coordinates), limit(coordinates), Zero<Value_type<V>>);
    }

    explicit constexpr
    point(C&& x)
        : coordinates{fw<C>(x)}
    {}

    constexpr auto
    operator()(Size_type<C> i) const -> S const&
    {
        return coordinates[i];
    }

    constexpr auto
    operator()(Size_type<C> i) -> S&
    {
        return coordinates[i];
    }
};

template <Range C>
point(C) -> point<Value_type<C>, size_type_t<C>::value, C>;

template <Semiring S, pointer_diff k, Range C, Vector_space V>
struct value_type_t<point<S, k, C, V>>
{
    using type = S;
};

template <Semiring S, pointer_diff k, Range C, Vector_space V>
struct vector_type_t<point<S, k, C, V>>
{
    using type = V;
};

template <Semiring S, pointer_diff k, Range C, Vector_space V>
struct vector_type_t<point<S, k, C, V> const>
{
    using type = V const;
};

template <Semiring S, pointer_diff k, Range C, Vector_space V>
struct scalar_type_t<point<S, k, C, V>>
{
    using type = S;
};

template <Semiring S, pointer_diff k, Range C, Vector_space V>
struct scalar_type_t<point<S, k, C, V> const>
{
    using type = S const;
};

template <typename S, pointer_diff k, typename C, typename V>
requires
    Semiring<S> and
    Sequence<C> and
    Vector_space<V>
struct difference_type_t<point<S, k, C, V>>
{
    using type = V;
};

template <Semiring S, pointer_diff k, Range C, Vector_space V>
struct size_type_t<point<S, k, C, V>>
{
    using type = Size_type<C>;
    static constexpr auto value = k;
};

template <Semiring S, pointer_diff k, Range C, Vector_space V>
constexpr auto
operator==(point<S, k, C, V> const& x, point<S, k, C, V> const& y) -> bool
{
    return x.coordinates == y.coordinates;
}

template <Semiring S, pointer_diff k, Sequence C, Vector_space V>
requires Default_totally_ordered<V>
struct lt<point<S, k, C, V>>
{
    constexpr auto
    operator()(point<S, k, C, V> const& x, point<S, k, C, V> const& y) -> bool
    {
        return lt{}(x.coordinates, y.coordinates);
    }
};

template <Semiring S, pointer_diff k, Range C, Vector_space V>
constexpr auto
is_empty(point<S, k, C, V> const&) -> bool
{
    return k == 0;
}

template <Semiring S, pointer_diff k, Range C, Vector_space V>
constexpr auto
size(point<S, k, C, V> const&) -> Size_type<point<S, k, C, V>>
{
    return k;
}

template <Semiring S, pointer_diff k, Range C, Vector_space V>
constexpr auto
operator-(point<S, k, C, V> const& x, point<S, k, C, V> const& y) -> V
{
    V v;
    map(first(x.coordinates), limit(x.coordinates), first(y.coordinates), first(v), sub_op{});
    return v;
}

template <Semiring S, pointer_diff k, Range C, Vector_space V>
constexpr auto
operator+(point<S, k, C, V> const& x, V const& v) -> point<S, k, C, V>
{
    point<S, k, C, V> y;
    map(first(x.coordinates), limit(x.coordinates), first(v), first(y.coordinates), add_op{});
    return y;
}

template <Semiring S, pointer_diff k, Range C, Vector_space V>
constexpr auto
operator+(V const& v, point<S, k, C, V> const& x) -> point<S, k, C, V>
{
    return x + v;
}

template <Ring S, pointer_diff k, Range C, Vector_space V>
constexpr auto
operator-(point<S, k, C, V> const& x, V const& v) -> point<S, k, C, V>
{
    point<S, k, C, V> y;
    map(first(x.coordinates), limit(x.coordinates), first(v), first(y.coordinates), sub_op{});
    return y;
}

template <Semiring S, pointer_diff k, Range C, Vector_space V>
constexpr auto
quadrance(point<S, k, C, V> const& x, point<S, k, C, V> const& y) -> Scalar_type<V>
{
    auto v{x - y};
    v = power_semigroup(v, S{2}, [](V& v0, V const& v1){ map(first(v0), limit(v0), first(v1), first(v0), mul_op<S>{}); });
    return reduce(first(v), limit(v), add_op<S>{}, Zero<S>);
}

}
