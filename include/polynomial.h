#pragma once

#include "array_single_ended.h"
#include "combinatorics.h"
#include "ordered_algebra.h"

namespace elements {

template <Ring R, Dynamic_sequence C = array_single_ended<R>>
struct polynomial
{
    C coefficients;

    polynomial()
        : coefficients{Zero<R>}
    {}

    explicit polynomial(std::initializer_list<R> coefficients_)
        : coefficients(coefficients_)
    {}

    polynomial(Size_type<C> const& size, R const& value)
        : coefficients(size, size, value)
    {}

    constexpr auto
    operator[](Size_type<C> i) const -> R const&
    {
        return coefficients[i];
    }

    constexpr auto
    operator[](Size_type<C> i) -> R&
    {
        return coefficients[i];
    }
};

template <Ring R, Dynamic_sequence C>
struct value_type_t<polynomial<R, C>>
{
    using type = R;
};

template <Ring R, Dynamic_sequence C>
struct size_type_t<polynomial<R, C>>
{
    using type = Size_type<C>;
};

template <Ring R, Dynamic_sequence C>
struct zero_type_t<polynomial<R, C>>
{
    static polynomial<R, C> const value;
};

template <Ring R, Dynamic_sequence C>
polynomial<R, C> const zero_type_t<polynomial<R, C>>::value = polynomial<R, C>{};

template <Ring R, Dynamic_sequence C>
struct one_type_t<polynomial<R, C>>
{
    static polynomial<R, C> const value;
};

template <Ring R, Dynamic_sequence C>
polynomial<R, C> const one_type_t<polynomial<R, C>>::value = polynomial<R, C>{One<R>};

template <Ring R, Dynamic_sequence C>
constexpr auto
operator==(polynomial<R, C> const& x, polynomial<R, C> const& y) -> bool
{
    auto dx = degree(x);
    auto dy = degree(y);
    return dx == dy and equal_lexicographical(
        first(x.coefficients), first(x.coefficients) + successor(dx),
        first(y.coefficients), first(y.coefficients) + successor(dy));
}

template <Ordered_ring R, Dynamic_sequence C>
constexpr auto
operator<(polynomial<R, C> const& x, polynomial<R, C> const& y) -> bool
{
    auto dx = degree(x);
    auto dy = degree(y);
    return
        dx < dy or (!(dy < dx) and less_lexicographical(
            first(x.coefficients), first(x.coefficients) + successor(dx),
            first(y.coefficients), first(y.coefficients) + successor(dy)));
}

template <Ring R, Dynamic_sequence C>
constexpr auto
operator+(polynomial<R, C> const& x, polynomial<R, C> const& y) -> polynomial<R, C>
{
    if (size(y.coefficients) < size(x.coefficients)) {
        polynomial<R, C> z(x);
        map(first(y.coefficients), limit(y.coefficients), first(z.coefficients), first(z.coefficients), sum{});
        return z;
    } else {
        polynomial<R, C> z(y);
        map(first(x.coefficients), limit(x.coefficients), first(z.coefficients), first(z.coefficients), sum{});
        return z;
    }
}

template <Ring R, Dynamic_sequence C>
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

template <Ring R, Dynamic_sequence C>
constexpr auto
operator-(polynomial<R, C> x) -> polynomial<R, C>
{
    map(first(x.coefficients), limit(x.coefficients), first(x.coefficients), negative<R>{});
    return x;
}

template <Ring R, Dynamic_sequence C>
constexpr auto
operator-(polynomial<R, C> const& x, polynomial<R, C> const& y) -> polynomial<R, C>
{
    return x + (-y);
}

template <Ring R, Dynamic_sequence C, typename I>
//requires Ordered_integral_domain<I>
constexpr auto
operator*(I const& n, polynomial<R, C> x) -> polynomial<R, C>
{
    map(
        first(x.coefficients), limit(x.coefficients), first(x.coefficients),
        [&](R const& y){ return n * y; });
    return x;
}

template <Ring R, Dynamic_sequence C, typename I>
//requires Ordered_integral_domain<I>
constexpr auto
operator*(polynomial<R, C> const& x, I const& n) -> polynomial<R, C>
{
    return n * x;
}

template <Ring R, Dynamic_sequence C>
constexpr auto
degree(polynomial<R, C> const& x) -> Size_type<polynomial<R, C>>
{
    return search_if(
        reverse_cursor(limit(x.coefficients)),
        reverse_cursor(first(x.coefficients)),
        [](R const& y){ return y != Zero<R>; }).cur - successor(first(x.coefficients));
}

template <Ring R, Dynamic_sequence C>
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
    Integral<I> and
    Dynamic_sequence<C>
constexpr auto
subderivative(polynomial<I, C> const& x, I const& k) -> polynomial<I, C>
    //[[expects: k < degree(p)]]
{
    if (k == Zero<I>) return x;
    polynomial<I, C> y(successor(degree(x)) - k, Zero<I>);
    auto cur0 = first(x.coefficients) + k;
    auto cur1 = first(y.coefficients);
    auto i = Zero<I>;
    while (cur1 != limit(y.coefficients)) {
        store(cur1, load(cur0) * choose(i + k, k));
        increment(cur0);
        increment(cur1);
        increment(i);
    }
    return y;
}

}
