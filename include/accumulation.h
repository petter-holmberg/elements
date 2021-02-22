#pragma once

#include "cursor.h"
#include "integer.h"

namespace elements {

template <typename F, typename Arg>
concept Accumulation_procedure =
    Invocable<F, Arg&, Arg> and
    Same_as<Return_type<F, Arg&, Arg>, void>;

template <Integer I, typename T, Operation<T, T> Op>
constexpr auto
power_left_associated(T a, I n, Op op) -> T
//[[expects: is_positive(n)]]
{
    if (n == One<I>) return a;
    return invoke(op, power_left_associated(a, n - One<I>, op), a);
}

template <Integer I, typename T, Accumulation_procedure<T> A>
constexpr auto
power_left_associated(T a, I n, A acc) -> T
//[[expects: is_positive(n)]]
{
    if (n == One<I>) return a;
    auto b{power_left_associated(a, n - One<I>, acc)};
    invoke(acc, b, a);
    return b;
}

template <Integer I, typename T, Operation<T, T> Op>
constexpr auto
power_right_associated(T a, I n, Op op) -> T
//[[expects: is_positive(n)]]
{
    if (n == One<I>) return a;
    return invoke(op, a, power_right_associated(a, n - One<I>, op));
}

template <Integer I, typename T, Accumulation_procedure<T> A>
constexpr auto
power_right_associated(T a, I n, A acc) -> T
//[[expects: is_positive(n)]]
{
    if (n == One<I>) return a;
    invoke(acc, a, power_right_associated(a, n - One<I>, acc));
    return a;
}

template <typename S, typename F>
concept Semigroup_accumulator =
    Regular<S> and
    Accumulation_procedure<F, S>;

template <Integer I, typename Op, Semigroup<Op> S>
constexpr auto
power_accumulate_semigroup(S r, S a, I n, Op op) -> S
//[[expects: axiom_Semigroup(a, a, a, op) and is_positive(n)]]
{
    while (true) {
        if (is_odd(n)) {
            r = invoke(op, r, a);
            if (is_one(n)) return r;
        }
        a = invoke(op, a, a);
        n = half(n);
    }
}

template <Integer I, typename A, Semigroup_accumulator<A> S>
constexpr auto
power_accumulate_semigroup(S r, S a, I n, A acc) -> S
//[[expects: axiom_Semigroup(a, a, a, acc) and is_positive(n)]]
{
    while (true) {
        if (is_odd(n)) {
            invoke(acc, r, a);
            if (is_one(n)) return r;
        }
        invoke(acc, a, a);
        n = half(n);
    }
}

template <Integer I, typename Op, Semigroup<Op> S>
constexpr auto
power_semigroup(S a, I n, Op op) -> S
//[[expects: axiom_Semigroup(a, a, a, op) and is_positive(n)]]
{
    while (is_even(n)) {
        a = invoke(op, a, a);
        n = half(n);
    }
    if (is_one(n)) return a;
    return power_accumulate_semigroup(a, invoke(op, a, a), half(n - One<I>), op);
}

template <Integer I, typename A, Semigroup_accumulator<A> S>
constexpr auto
power_semigroup(S a, I n, A acc) -> S
//[[expects: axiom_Semigroup(a, a, a, acc) and is_positive(n)]]
{
    while (is_even(n)) {
        invoke(acc, a, a);
        n = half(n);
    }
    if (is_one(n)) return a;
    auto b{a};
    invoke(acc, b, a);
    return power_accumulate_semigroup(a, b, half(n - One<I>), acc);
}

template <typename M, typename F>
concept Monoid_accumulator =
    Semigroup_accumulator<M, F> and
    requires {
        M{Identity_element<M, F>};
    };

template <Integer I, typename Op, Monoid<Op> M>
constexpr auto
power_monoid(M a, I n, Op op) -> M
//[[expects: axiom_Monoid(a, a, a, op) and !is_negative(n)]]
{
    if (is_zero(n)) return Identity_element<M, Op>;
    return power_semigroup(a, n, op);
}

template <Integer I, typename A, Monoid_accumulator<A> M>
constexpr auto
power_monoid(M a, I n, A acc) -> M
//[[expects: axiom_Monoid(a, a, a, acc) and !is_negative(n)]]
{
    if (is_zero(n)) return Identity_element<M, A>;
    return power_semigroup(a, n, acc);
}

template <typename M, typename F>
requires Monoid_accumulator<M, F>
struct inverse_accumulator;

template <typename G, typename F>
concept Group_accumulator =
    Monoid_accumulator<G, F> and
    Invocable<inverse_accumulator<G, F>, G&> and
    Same_as<Return_type<inverse_accumulator<G, F>, G&>, void>;

template <Integer I, typename Op, Group<Op> G>
constexpr auto
power_group(G a, I n, Op op) -> G
//[[expects: axiom_Group(a, a, a, op)]]
{
    if (is_negative(n)) {
        n = negative(n);
        a = inverse<G, Op>(a);
    }
    return power_monoid(a, n, op);
}

template <Integer I, typename A, Group_accumulator<A> G>
constexpr auto
power_group(G a, I n, A acc) -> G
//[[expects: axiom_Group(a, a, a, acc)]]
{
    if (is_negative(n)) {
        n = negative(n);
        inverse_accumulator<G, A>{}(a);
    }
    return power_monoid(a, n, acc);
}

}
