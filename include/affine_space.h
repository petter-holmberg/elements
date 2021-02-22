#pragma once

#include "array_k.h"
#include "fill.h"
#include "reduce.h"

namespace elements {

template <typename P, typename V = Difference_type<P>, typename S = Value_type<V>>
concept Affine_space =
    Regular<P> and
    Vector_space<V, S> and
    requires (P const& p, V const& v) {
        { p - p } -> Same_as<V>;
        { p + v } -> Same_as<P>;
        { v + p } -> Same_as<P>;
        { p - v } -> Same_as<P>;
    };

template <typename P, typename V = Difference_type<P>, typename S = Value_type<V>>
requires Affine_space<P>
constexpr auto
operator+=(P& p, V const& v) -> P&
{
    p = p + v;
    return p;
}

template <typename P, typename V = Difference_type<P>, typename S = Value_type<V>>
requires Affine_space<P>
constexpr auto
operator-=(P& p, V const& v) -> P&
{
    p = p - v;
    return p;
}

template <
    typename S, int32_t k,
    typename E = array_k<S, k>,
    typename S_add_op = add_op<S>,
    typename S_mul_op = mul_op<S>>
requires
    Semiring<S, S_add_op, S_mul_op> and
    Sequence<E>
struct affine_vector
{
    E elements;

    constexpr
    affine_vector()
        : elements{}
    {
        fill(first(elements), limit(elements), Zero<S>);
    }

    constexpr
    affine_vector(std::initializer_list<S> x) : elements{x} {}

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

template <typename S, int32_t k, typename E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
struct scalar_type_t<affine_vector<S, k, E, S_add_op, S_mul_op>>
{
    using type = S;
};

template <typename S, int32_t k, typename E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
struct value_type_t<affine_vector<S, k, E, S_add_op, S_mul_op>>
{
    using type = S;
};

template <typename S, int32_t k, typename E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
struct cursor_type_t<affine_vector<S, k, E, S_add_op, S_mul_op>>
{
    using type = Cursor_type<E>;
};

template <typename S, int32_t k, typename E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
struct cursor_type_t<affine_vector<S, k, E, S_add_op, S_mul_op> const>
{
    using type = Cursor_type<E const>;
};

template <typename S, int32_t k, typename E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
struct size_type_t<affine_vector<S, k, E, S_add_op, S_mul_op>>
{
    using type = Size_type<E>;
    static constexpr auto value = k;
};

template <typename S, int32_t k, Regular E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
operator==(
    affine_vector<S, k, E, S_add_op, S_mul_op> const& x,
    affine_vector<S, k, E, S_add_op, S_mul_op> const& y) -> bool
{
    return x.elements == y.elements;
}

template <typename S, int32_t k, Default_totally_ordered E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
struct lt<affine_vector<S, k, E, S_add_op, S_mul_op>>
{
    constexpr auto
    operator()(
        affine_vector<S, k, E, S_add_op, S_mul_op> const& x,
        affine_vector<S, k, E, S_add_op, S_mul_op> const& y) -> bool
    {
        return lt{}(x.elements, y.elements);
    }
};

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
first(affine_vector<S, k, E, S_add_op, S_mul_op> const& x) -> Cursor_type<E const>
{
    return first(x.elements);
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
first(affine_vector<S, k, E, S_add_op, S_mul_op>& x) -> Cursor_type<E>
{
    return first(x.elements);
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
limit(affine_vector<S, k, E, S_add_op, S_mul_op> const& x) -> Cursor_type<E const>
{
    return limit(x.elements);
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
limit(affine_vector<S, k, E, S_add_op, S_mul_op>& x) -> Cursor_type<E>
{
    return limit(x.elements);
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
is_empty(affine_vector<S, k, E, S_add_op, S_mul_op> const&) -> bool
{
    return k == 0;
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
size(
    affine_vector<S, k, E, S_add_op, S_mul_op> const&) ->
    Size_type<affine_vector<S, k, E, S_add_op, S_mul_op>>
{
    return k;
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
operator+(
    affine_vector<S, k, E, S_add_op, S_mul_op> const& x,
    affine_vector<S, k, E, S_add_op, S_mul_op> const& y) ->
    affine_vector<S, k, E, S_add_op, S_mul_op>
{
    affine_vector<S, k, E, S_add_op, S_mul_op> z;
    map(first(x), limit(x), first(y), first(z), S_add_op{});
    return z;
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Ring<S, S_add_op, S_mul_op>
constexpr auto
operator-(
    affine_vector<S, k, E, S_add_op, S_mul_op> const& x) ->
    affine_vector<S, k, E, S_add_op, S_mul_op>
{
    affine_vector<S, k, E, S_add_op, S_mul_op> y;
    map(first(x), limit(x), first(y), inverse_op<S, S_add_op>{});
    return y;
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Ring<S, S_add_op, S_mul_op>
constexpr auto
operator-(
    affine_vector<S, k, E, S_add_op, S_mul_op> const& x,
    affine_vector<S, k, E, S_add_op, S_mul_op> const& y) ->
    affine_vector<S, k, E, S_add_op, S_mul_op>
{
    affine_vector<S, k, E, S_add_op, S_mul_op> z;
    map(first(x), limit(x), first(y), first(z), difference{});
    return z;
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
operator*(
    affine_vector<S, k, E, S_add_op, S_mul_op> const& x,
    affine_vector<S, k, E, S_add_op, S_mul_op> const& y) -> S
{
    affine_vector<S, k, E, S_add_op, S_mul_op> z;
    return inner_product(first(x), limit(x), first(y), first(z), S_add_op{}, S_mul_op{});
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
operator*(
    affine_vector<S, k, E, S_add_op, S_mul_op> const& x, S const& n) ->
    affine_vector<S, k, E, S_add_op, S_mul_op>
{
    affine_vector<S, k, E, S_add_op, S_mul_op> z;
    S_mul_op mul{};
    map(first(x), limit(x), first(z), [&n, &mul](S const& x_){ return mul(x_, n); });
    return z;
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
operator*(
    S const& n, affine_vector<S, k, E, S_add_op, S_mul_op> const& x) ->
    affine_vector<S, k, E, S_add_op, S_mul_op>
{
    return x * n;
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Field<S, S_add_op, S_mul_op>
constexpr auto
operator/(
    affine_vector<S, k, E, S_add_op, S_mul_op> const& x, S const& n) ->
    affine_vector<S, k, E, S_add_op, S_mul_op>
{
    affine_vector<S, k, E, S_add_op, S_mul_op> z;
    map(first(x), limit(x), first(z), [&n](S const& x_){ return x_ / n; });
    return z;
}

template <
    typename S,
    int32_t k,
    typename C = array_k<S, k>,
    typename V = affine_vector<S, k, C>>
requires
    Field<S> and
    Sequence<C> and
    Vector_space<V> and
    Sequence<V>
struct affine_point
{
    C coordinates;

    constexpr
    affine_point()
        : coordinates{}
    {
        fill(first(coordinates), limit(coordinates), Zero<Value_type<V>>);
    }

    constexpr
    affine_point(std::initializer_list<S> x) : coordinates{x} {}

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
struct value_type_t<affine_point<S, k, C, V>>
{
    using type = S;
};

template <typename S, int32_t k, typename C, typename V>
requires
    Field<S> and
    Sequence<C> and
    Vector_space<V> and
    Sequence<V>
struct difference_type_t<affine_point<S, k, C, V>>
{
    using type = V;
};

template <typename S, int32_t k, typename C, typename V>
requires
    Field<S> and
    Sequence<C> and
    Vector_space<V> and
    Sequence<V>
struct cursor_type_t<affine_point<S, k, C, V>>
{
    using type = Cursor_type<C>;
};

template <typename S, int32_t k, typename C, typename V>
requires
    Field<S> and
    Sequence<C> and
    Vector_space<V> and
    Sequence<V>
struct cursor_type_t<affine_point<S, k, C, V> const>
{
    using type = Cursor_type<C const>;
};

template <typename S, int32_t k, typename C, typename V>
requires
    Field<S> and
    Sequence<C> and
    Vector_space<V> and
    Sequence<V>
struct size_type_t<affine_point<S, k, C, V>>
{
    using type = Size_type<C>;
    static constexpr auto value = k;
};

template <typename S, int32_t k, typename C, Regular V>
constexpr auto
operator==(affine_point<S, k, C, V> const& x, affine_point<S, k, C, V> const& y) -> bool
{
    return x.coordinates == y.coordinates;
}

template <typename S, int32_t k, typename C, Default_totally_ordered V>
struct lt<affine_point<S, k, C, V>>
{
    constexpr auto
    operator()(affine_point<S, k, C, V> const& x, affine_point<S, k, C, V> const& y) -> bool
    {
        return lt{}(x.coordinates, y.coordinates);
    }
};

template <typename S, int32_t k, Range C, typename V>
constexpr auto
first(affine_point<S, k, C, V> const& x) -> Cursor_type<V const>
{
    return first(x.coordinates);
}

template <typename S, int32_t k, Range C, typename V>
constexpr auto
first(affine_point<S, k, C, V>& x) -> Cursor_type<V>
{
    return first(x.coordinates);
}

template <typename S, int32_t k, Range C, typename V>
constexpr auto
limit(affine_point<S, k, C, V> const& x) -> Cursor_type<V const>
{
    return limit(x.coordinates);
}

template <typename S, int32_t k, Range C, typename V>
constexpr auto
limit(affine_point<S, k, C, V>& x) -> Cursor_type<V>
{
    return limit(x.coordinates);
}

template <typename S, int32_t k, Range C, typename V>
constexpr auto
is_empty(affine_point<S, k, C, V> const&) -> bool
{
    return k == 0;
}

template <typename S, int32_t k, Range C, typename V>
constexpr auto
size(affine_point<S, k, C, V> const&) -> Size_type<affine_point<S, k, C, V>>
{
    return k;
}

template <typename S, int32_t k, Range C, typename V>
requires
    Field<S> and
    Vector_space<V> and
    Sequence<V>
constexpr auto
operator-(affine_point<S, k, C, V> const& x, affine_point<S, k, C, V> const& y) -> V
{
    V v;
    map(first(x), limit(x), first(y), first(v), difference{});
    return v;
}

template <typename S, int32_t k, Range C, typename V>
requires
    Field<S> and
    Vector_space<V> and
    Sequence<V>
constexpr auto
operator+(affine_point<S, k, C, V> const& x, V const& v) -> affine_point<S, k, C, V>
{
    affine_point<S, k, C, V> y;
    map(first(x), limit(x), first(v), first(y), [](S const& a, S const& b){ return a + b; });
    return y;
}

template <typename S, int32_t k, Range C, typename V>
requires
    Field<S> and
    Vector_space<V> and
    Sequence<V>
constexpr auto
operator+(V const& v, affine_point<S, k, C, V> const& x) -> affine_point<S, k, C, V>
{
    return x + v;
}

template <typename S, int32_t k, Range C, typename V>
requires
    Field<S> and
    Vector_space<V> and
    Sequence<V>
constexpr auto
operator-(affine_point<S, k, C, V> const& x, V const& v) -> affine_point<S, k, C, V>
{
    affine_point<S, k, C, V> y;
    map(first(x), limit(x), first(v), first(y), [](S const& a, S const& b){ return a - b; });
    return y;
}

}
