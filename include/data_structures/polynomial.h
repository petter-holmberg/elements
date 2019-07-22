#pragma once

#include "array_k.h"
#include "adapters/reverse_position.h"
#include "algorithms/algebra.h"
#include "algorithms/map.h"
#include "algorithms/ordered_algebra.h"
#include "algorithms/regular.h"
#include "algorithms/search.h"
#include "algorithms/lexicographical.h"

namespace elements {

template <
    typename R,
    typename C = array<R>>
requires
    Ring<R> and
    Dynamic_sequence<C>
struct polynomial
{
    array<R> coefficients;

    polynomial()
        : coefficients{Zero<R>}
    {}

    explicit polynomial(std::initializer_list<R> coefficients_)
        : coefficients(coefficients_)
    {}

    polynomial(Size_type<array<R>> const& size, R const& value)
        : coefficients(size, size, value)
    {}

    constexpr auto
    operator[](Size_type<array<R>> i) const -> R const&
    {
        return coefficients[i];
    }

    constexpr auto
    operator[](Size_type<array<R>> i) -> R&
    {
        return coefficients[i];
    }
};

template <typename R, typename C>
requires
    Ring<R> and
    Dynamic_sequence<C>
struct underlying_type_t<polynomial<R, C>>
{
    using type = array<R>;
};

template <typename R, typename C>
requires
    Ring<R> and
    Dynamic_sequence<C>
struct value_type_t<polynomial<R, C>>
{
    using type = R;
};

template <typename R, typename C>
requires
    Ring<R> and
    Dynamic_sequence<C>
struct size_type_t<polynomial<R, C>>
{
    using type = Size_type<Underlying_type<polynomial<R, C>>>;
};

template <typename R, typename C>
requires
    Ring<R> and
    Dynamic_sequence<C>
struct zero_type_t<polynomial<R, C>>
{
    static polynomial<R, C> const value;
};

template <typename R, typename C>
requires
    Ring<R> and
    Dynamic_sequence<C>
polynomial<R, C> const zero_type_t<polynomial<R, C>>::value = polynomial<R, C>{};

template <typename R, typename C>
requires
    Ring<R> and
    Dynamic_sequence<C>
struct one_type_t<polynomial<R, C>>
{
    static polynomial<R, C> const value;
};

template <typename R, typename C>
requires
    Ring<R> and
    Dynamic_sequence<C>
polynomial<R, C> const one_type_t<polynomial<R, C>>::value = polynomial<R, C>{One<R>};

template <typename R, typename C>
requires
    Ring<R> and
    Dynamic_sequence<C>
constexpr auto
operator==(polynomial<R, C> const& x, polynomial<R, C> const& y) -> bool
{
    auto dx = degree(x);
    auto dy = degree(y);
    return dx == dy and lexicographical_equal(
        first(x.coefficients), first(x.coefficients) + successor(dx),
        first(y.coefficients), first(y.coefficients) + successor(dy));
}

template <typename R, typename C>
requires
    Ordered_ring<R> and
    Dynamic_sequence<C>
constexpr auto
operator<(polynomial<R, C> const& x, polynomial<R, C> const& y) -> bool
{
    auto dx = degree(x);
    auto dy = degree(y);
    return
        dx < dy or (!(dy < dx) and lexicographical_less(
            first(x.coefficients), first(x.coefficients) + successor(dx),
            first(y.coefficients), first(y.coefficients) + successor(dy)));
}

template <typename R, typename C>
requires
    Ring<R> and
    Dynamic_sequence<C>
constexpr auto
operator+(polynomial<R, C> const& x, polynomial<R, C> const& y) -> polynomial<R, C>
{
    if (size(y.coefficients) < size(x.coefficients)) {
        polynomial<R, C> z(x);
        map(first(y.coefficients), limit(y.coefficients), first(z.coefficients), first(z.coefficients), sum<R>{});
        return z;
    } else {
        polynomial<R, C> z(y);
        map(first(x.coefficients), limit(x.coefficients), first(z.coefficients), first(z.coefficients), sum<R>{});
        return z;
    }
}

template <typename R, typename C>
requires
    Ring<R> and
    Dynamic_sequence<C>
constexpr auto
operator*(polynomial<R, C> const& x, polynomial<R, C> const& y) -> polynomial<R, C>
{
    if (x == Zero<polynomial<R, C>>) return Zero<polynomial<R, C>>;
    if (y == Zero<polynomial<R, C>>) return Zero<polynomial<R, C>>;

    if (x == One<polynomial<R, C>>) return y;
    if (y == One<polynomial<R, C>>) return x;

    auto dx = degree(x);
    auto dy = degree(y);
    polynomial<R, C> z(dx + successor(dy), Zero<R>);

    auto src0 = first(x.coefficients);
    auto lim0 = src0 + successor(dx);
    auto dst_begin = first(z.coefficients);
    while (src0 != lim0) {
        auto src1 = first(y.coefficients);
        auto lim1 = src1 + successor(dy);
        auto dst = dst_begin;
        while (src1 != lim1) {
            store(dst, load(dst) + load(src0) * load(src1));
            increment(src1);
            increment(dst);
        }
        increment(src0);
        increment(dst_begin);
    }

    return z;
}

template <typename R, typename C>
requires
    Ring<R> and
    Dynamic_sequence<C>
constexpr auto
operator-(polynomial<R, C> x) -> polynomial<R, C>
{
    map(first(x.coefficients), limit(x.coefficients), first(x.coefficients), negative<R>{});
    return x;
}

template <typename R, typename C>
requires
    Ring<R> and
    Dynamic_sequence<C>
constexpr auto
operator-(polynomial<R, C> const& x, polynomial<R, C> const& y) -> polynomial<R, C>
{
    return x + (-y);
}

template <typename R, typename C, typename I>
requires
    Ring<R> and
    Dynamic_sequence<C>
// and Ordered_integral_domain<I>
constexpr auto
operator*(I const& n, polynomial<R, C> x) -> polynomial<R, C>
{
    map(
        first(x.coefficients), limit(x.coefficients), first(x.coefficients),
        [&](R const& y){ return n * y; });
    return x;
}

template <typename R, typename C, typename I>
requires
    Ring<R> and
    Dynamic_sequence<C>
// and Ordered_integral_domain<I>
constexpr auto
operator*(polynomial<R, C> const& x, I const& n) -> polynomial<R, C>
{
    return n * x;
}

template <typename R, typename C>
requires
    Ring<R> and
    Dynamic_sequence<C>
constexpr auto
degree(polynomial<R, C> const& x) -> Size_type<polynomial<R, C>>
{
    return search_if(
        reverse_position(limit(x.coefficients)),
        reverse_position(first(x.coefficients)),
        [](R const& y){ return y != Zero<R>; }).pos - successor(first(x.coefficients));
}

template <typename R, typename C>
requires
    Ring<R> and
    Dynamic_sequence<C>
constexpr auto
evaluate(polynomial<R, C> const& p, R const& x) -> R
{
    auto k = degree(p);
    auto r = p.coefficients[k];
    while (k > 0) {
        decrement(k);
        r = (r * x) + p.coefficients[k];
    }
    return r;
}

template <typename I, typename C>
requires
    Binary_integer<I> and
    Dynamic_sequence<C>
constexpr auto
subderivative(polynomial<I, C> const& x, I const& k) -> polynomial<I, C>
    //[[expects: k < degree(p)]]
{
    if (k == Zero<I>) return x;
    polynomial<I, C> y(successor(degree(x)) - k, Zero<I>);
    auto pos0 = first(x.coefficients) + k;
    auto pos1 = first(y.coefficients);
    auto i = Zero<I>;
    while (pos1 != limit(y.coefficients)) {
        store(pos1, load(pos0) * choose(i + k, k));
        increment(pos0);
        increment(pos1);
        increment(i);
    }
    return y;
}

}
