#pragma once

#include "algebra.h"
#include "ordering.h"
#include "pair.h"

namespace elements {

template <typename S, typename Op>
concept Ordered_semigroup =
    Semigroup<S, Op> and
    Totally_ordered<S>;
    // axiom natural_total_order {
    //     a < b => a + c < b + c;
    // }

template <typename S>
concept Ordered_additive_semigroup =
    Additive_semigroup<S> and
    Totally_ordered<S>;
    // axiom natural_total_order {
    //     a < b => a + c < b + c;
    // }

template <typename M, typename Op>
concept Ordered_monoid =
    Monoid<M, Op> and
    Totally_ordered<M>;

template <typename M>
concept Ordered_additive_monoid =
    Additive_monoid<M> and
    Totally_ordered<M>;

template <typename G, typename Op>
concept Ordered_group =
    Monoid<G, Op> and
    Totally_ordered<G>;

template <typename G>
concept Ordered_additive_group =
    Additive_monoid<G> and
    Totally_ordered<G>;

template <typename G, typename Op = add_op<G>>
requires Ordered_group<G, Op>
constexpr auto
abs(G const& a) -> G
{
    if (a < G{0}) return inverse<G, Op>(a);
    else return a;
}

template <Integral_domain I>
constexpr auto
cancel(I const& a, I const& b) -> I
{
    return a - b;
}

template <typename M, typename Op>
concept Cancellable_monoid =
    Ordered_monoid<M, Op> and
    requires (M const& a, M const& b) {
        { cancel(a, b) } -> Same_as<M>;
    };
    // axiom cancellation {
    //     b <= a => op(cancel(a, b), b) = a
    // }

template <typename T>
struct quotient_type_t
{
    using type = T;
};

template <typename T>
using Quotient_type = typename quotient_type_t<T>::type;

template <typename M, typename Op>
concept Archimedean_monoid =
    Cancellable_monoid<M, Op> and
    Integral_domain<Quotient_type<M>>;

template <typename M, typename Op = add_op<M>>
requires Archimedean_monoid<M, Op>
constexpr auto
largest_doubling(M a, M b, Op op = {}) -> M
//[[expects: a >= b > Zero<M>]]
{
    while (b <= cancel(a, b)) b = op(b, b);
    return b;
}

template <typename M, typename Op>
concept Halvable_monoid =
    Archimedean_monoid<M, Op> and
    requires (M a) {
        { half(a) } -> Same_as<M>;
    };
    // axiom {
    //     b > 0 and a = b + b => half(a) = b
    // }

template <typename M, typename Op = add_op<M>>
requires Halvable_monoid<M, Op>
constexpr auto
quotient(M a, M b, Op op = {}) -> M
//[[expects: a >= Zero<M> and b > Zero<M>]]
{
    if constexpr(Integral<M> and Same_as<Op, add_op<M>>) {
        return a / b;
    } else {
        if (a < b) return Zero<M>;
        auto c{largest_doubling(a, b, op)};
        auto n{One<M>};
        a = cancel(a, c);
        while (c != b) {
            c = half(c);
            n = twice(n);
            if (c <= a) {
                a = cancel(a, c);
                n = n + One<M>;
            }
        }
        return n;
    }
}

template <typename M, typename Op = add_op<M>>
requires Halvable_monoid<M, Op>
constexpr auto
remainder(M a, M b, Op op = {}) -> M
//[[expects: a >= Zero<M> and b > Zero<M>]]
{
    if constexpr(Integral<M> and Same_as<Op, add_op<M>>) {
        return a % b;
    } else {
        if (a < b) return a;
        auto c{largest_doubling(a, b, op)};
        a = cancel(a, c);
        while (c != b) {
            c = half(c);
            if (c <= a) a = cancel(a, c);
        }
        return a;
    }
}

template <typename G, typename Op>
concept Archimedean_group =
    Archimedean_monoid<G, Op> and
    Group<G, Op>;

template <typename M, typename Op = add_op<M>>
requires Archimedean_monoid<M, Op>
constexpr auto
quotient_remainder(M a, M b, Op op = {}) -> pair<Quotient_type<M>, M>
//[[expects: a >= Zero<M, Op> and b > Zero<M, Op>]]
{
    if (a < b) return {Zero<Quotient_type<M>>, a};
    auto c{cancel(a, b)};
    if (c < b) return {One<Quotient_type<M>>, c};
    auto q{quotient_remainder(a, op(b, b), op)};
    auto m{twice(q.m0)};
    a = q.m1;
    if (a < b) {
        return {m, a};
    } else {
        return {successor(m), cancel(a, b)};
    }
}

template <typename G, typename Op = add_op<G>>
requires Archimedean_group<G, Op>
constexpr auto
quotient_remainder(G a, G b, Op op = {}) -> pair<Quotient_type<G>, G>
//[[expects: a >= Identity_element<G, Op> and b > Identity_element<G, Op>]]
{
    if (a < b) return {Zero<Quotient_type<G>>, a};
    auto c{inverse<G, Op>(a, b)};
    if (c < b) return {One<Quotient_type<G>>, c};
    auto q{quotient_remainder(a, op(b, b), op)};
    auto m{twice(q.m0)};
    a = q.m1;
    if (a < b) {
        return {m, a};
    } else {
        return {successor(m), inverse<G, Op>(a, b)};
    }
}

template <
    typename V, typename V_add_op = add_op<V>,
    typename S = Value_type<V>, typename S_add_op = add_op<S>, typename S_mul_op = mul_op<S>>
concept Euclidean_semimodule =
    Semimodule<V, V_add_op, S, S_add_op, S_mul_op> and
    requires (V const& a) {
        { remainder(a, a) } -> Same_as<V>;
        { quotient(a, a) } -> Same_as<S>;
    };

template <
    typename V, typename V_add_op = add_op<V>,
    typename S = Value_type<V>, typename S_add_op = add_op<S>, typename S_mul_op = mul_op<S>>
requires Euclidean_semimodule<V, V_add_op, S, S_add_op, S_mul_op>
constexpr auto
gcd(V a, V b) -> V
{
    while (true) {
        if (b == Zero<V>) return a;
        a = remainder(a, b);
        if (a == Zero<V>) return b;
        b = remainder(b, a);
    }
    return a;
}

template <typename R>
concept Ordered_ring =
    Ring<R> and
    Totally_ordered<R>;

template <typename I>
concept Ordered_integral_domain =
    Integral_domain<I> and
    Totally_ordered<I>;

}
