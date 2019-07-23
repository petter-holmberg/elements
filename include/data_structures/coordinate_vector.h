#pragma once

#include "array_k.h"
#include "algorithms/algebra.h"
#include "algorithms/fill.h"
#include "algorithms/map.h"
#include "algorithms/regular.h"

namespace elements {

template <
    typename S, int32_t k,
    typename E = array_k<S, k>,
    typename S_add_op = sum<S>,
    typename S_mul_op = product<S>>
requires
    Semiring<S, S_add_op, S_mul_op> and
    Sequence<E>
struct coordinate_vector
{
    E elements;

    constexpr
    coordinate_vector()
        : elements{}
    {
        fill(first(elements), limit(elements), Zero<S>);
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

template <typename S, int32_t k, typename E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
struct underlying_type_t<coordinate_vector<S, k, E, S_add_op, S_mul_op>>
{
    using type = coordinate_vector<Underlying_type<S>, k, Underlying_type<E>, S_add_op, S_mul_op>;
};

template <typename S, int32_t k, typename E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
struct value_type_t<coordinate_vector<S, k, E, S_add_op, S_mul_op>>
{
    using type = S;
};

template <typename S, int32_t k, typename E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
struct position_type_t<coordinate_vector<S, k, E, S_add_op, S_mul_op>>
{
    using type = Position_type<E>;
};

template <typename S, int32_t k, typename E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
struct position_type_t<coordinate_vector<S, k, E, S_add_op, S_mul_op> const>
{
    using type = Position_type<E const>;
};

template <typename S, int32_t k, typename E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
struct size_type_t<coordinate_vector<S, k, E, S_add_op, S_mul_op>>
{
    using type = Size_type<E>;
    static constexpr auto value = k;
};

template <typename S, int32_t k, Regular E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
operator==(
    coordinate_vector<S, k, E, S_add_op, S_mul_op> const& x,
    coordinate_vector<S, k, E, S_add_op, S_mul_op> const& y) -> bool
{
    return x.elements == y.elements;
}

template <typename S, int32_t k, Default_totally_ordered E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
struct less<coordinate_vector<S, k, E, S_add_op, S_mul_op>>
{
    constexpr auto
    operator()(
        coordinate_vector<S, k, E, S_add_op, S_mul_op> const& x,
        coordinate_vector<S, k, E, S_add_op, S_mul_op> const& y) -> bool
    {
        return less<E>{}(x.elements, y.elements);
    }
};

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
first(coordinate_vector<S, k, E, S_add_op, S_mul_op> const& x) -> Position_type<E const>
{
    return first(x.elements);
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
first(coordinate_vector<S, k, E, S_add_op, S_mul_op>& x) -> Position_type<E>
{
    return first(x.elements);
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
limit(coordinate_vector<S, k, E, S_add_op, S_mul_op> const& x) -> Position_type<E const>
{
    return limit(x.elements);
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
limit(coordinate_vector<S, k, E, S_add_op, S_mul_op>& x) -> Position_type<E>
{
    return limit(x.elements);
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
is_empty(coordinate_vector<S, k, E, S_add_op, S_mul_op> const&) -> bool
{
    return k == 0;
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
size(
    coordinate_vector<S, k, E, S_add_op, S_mul_op> const&) ->
    Size_type<coordinate_vector<S, k, E, S_add_op, S_mul_op>>
{
    return k;
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
operator+(
    coordinate_vector<S, k, E, S_add_op, S_mul_op> const& x,
    coordinate_vector<S, k, E, S_add_op, S_mul_op> const& y) ->
    coordinate_vector<S, k, E, S_add_op, S_mul_op>
{
    coordinate_vector<S, k, E, S_add_op, S_mul_op> z;
    map(first(x), limit(x), first(y), first(z), S_add_op{});
    return z;
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Ring<S, S_add_op, S_mul_op>
constexpr auto
operator-(
    coordinate_vector<S, k, E, S_add_op, S_mul_op> const& x) ->
    coordinate_vector<S, k, E, S_add_op, S_mul_op>
{
    coordinate_vector<S, k, E, S_add_op, S_mul_op> y;
    map(first(x), limit(x), first(y), inverse_operation<S, S_add_op>{});
    return y;
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Ring<S, S_add_op, S_mul_op>
constexpr auto
operator-(
    coordinate_vector<S, k, E, S_add_op, S_mul_op> const& x,
    coordinate_vector<S, k, E, S_add_op, S_mul_op> const& y) ->
    coordinate_vector<S, k, E, S_add_op, S_mul_op>
{
    coordinate_vector<S, k, E, S_add_op, S_mul_op> z;
    map(first(x), limit(x), first(y), first(z), difference<S>{});
    return z;
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
operator*(
    coordinate_vector<S, k, E, S_add_op, S_mul_op> const& x,
    coordinate_vector<S, k, E, S_add_op, S_mul_op> const& y) -> S
{
    coordinate_vector<S, k, E, S_add_op, S_mul_op> z;
    return inner_product(first(x), limit(x), first(y), first(z), S_add_op{}, S_mul_op{});
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
operator*(
    coordinate_vector<S, k, E, S_add_op, S_mul_op> const& x, S const& n) ->
    coordinate_vector<S, k, E, S_add_op, S_mul_op>
{
    coordinate_vector<S, k, E, S_add_op, S_mul_op> z;
    S_mul_op mul{};
    map(first(x), limit(x), first(z), [&n, &mul](S const& x_){ return mul(x_, n); });
    return z;
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Semiring<S, S_add_op, S_mul_op>
constexpr auto
operator*(
    S const& n, coordinate_vector<S, k, E, S_add_op, S_mul_op> const& x) ->
    coordinate_vector<S, k, E, S_add_op, S_mul_op>
{
    return x * n;
}

template <typename S, int32_t k, Range E, typename S_add_op, typename S_mul_op>
requires Field<S, S_add_op, S_mul_op>
constexpr auto
operator/(
    coordinate_vector<S, k, E, S_add_op, S_mul_op> const& x, S const& n) ->
    coordinate_vector<S, k, E, S_add_op, S_mul_op>
{
    coordinate_vector<S, k, E, S_add_op, S_mul_op> z;
    map(first(x), limit(x), first(z), [&n](S const& x_){ return x_ / n; });
    return z;
}

}
