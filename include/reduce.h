#pragma once

#include "binary_counter.h"

namespace elements {

template <Cursor C, Limit<C> L, Regular_invocable<C> F, Operation<Return_type<F, C>, Return_type<F, C>> Op>
constexpr auto
reduce_nonempty(C cur, L lim, Op op, F fun) -> Return_type<F, C>
//[[expects axiom: range(cur, lim)]]
//[[expects: precedes(cur, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    auto x = invoke(fun, cur);
    increment(cur);
    while (precedes(cur, lim)) {
        store(x, op(x, invoke(fun, cur)));
        increment(cur);
    }
    return x;
}

template <Cursor C, Limit<C> L, Operation<Value_type<C>, Value_type<C>> Op>
requires Loadable<C>
constexpr auto
reduce_nonempty(C cur, L lim, Op op) -> Value_type<C>
//[[expects axiom: loadable_range(cur, lim)]]
//[[expects: precedes(cur, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    return reduce_nonempty(mv(cur), mv(lim), op, [](C const& c){ return load(c); });
}

template <Cursor C, Limit<C> L, Regular_invocable<C> F, Operation<Return_type<F, C>, Return_type<F, C>> Op>
requires Loadable<C>
constexpr auto
reduce(C cur, L lim, Op op, F fun, Return_type<F, C> const& zero) -> Return_type<F, C>
//[[expects axiom: range(cur, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    if (!precedes(cur, lim)) return zero;
    return reduce_nonempty(mv(cur), mv(lim), op, fun);
}

template <Cursor C, Limit<C> L, Operation<Value_type<C>, Value_type<C>> Op>
requires Loadable<C>
constexpr auto
reduce(C cur, L lim, Op op, Value_type<C> const& zero) -> Value_type<C>
//[[expects axiom: loadable_range(cur, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    return reduce(mv(cur), mv(lim), op, [](C const& c){ return load(c); }, zero);
}

template <Cursor C, Limit<C> L, Regular_invocable<C> F, Operation<Return_type<F, C>, Return_type<F, C>> Op>
constexpr auto
reduce_nonzeroes(C cur, L lim, Op op, F fun, Return_type<F, C> const& zero) -> Return_type<F, C>
//[[expects axiom: range(cur, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    Return_type<F, C> x;
    do {
        if (!(precedes(cur, lim))) return zero;
        store(x, invoke(fun, cur));
        increment(cur);
    } while (x == zero);
    while (precedes(cur, lim)) {
        auto y = invoke(fun, cur);
        if (y != zero) store(x, op(x, y));
        increment(cur);
    }
    return x;
}

template <Cursor C, Limit<C> L, Operation<Value_type<C>, Value_type<C>> Op>
requires Loadable<C>
constexpr auto
reduce_nonzeroes(C cur, L lim, Op op, Value_type<C> const& zero) -> Value_type<C>
//[[expects axiom: loadable_range(cur, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    return reduce_nonzeroes(mv(cur), mv(lim), op, [](C const& c){ return load(c); }, zero);
}

template <Cursor C, Limit<C> L, Regular_invocable<C> F, Operation<Return_type<F, C>, Return_type<F, C>> Op>
constexpr auto
reduce_balanced(C cur, L lim, Op op, F fun, Value_type<C> const& zero) -> Return_type<F, C>
//[[expects axiom: loadable_range(cur, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    binary_counter<Value_type<C>, Op> counter(op, zero);
    while (precedes(cur, lim)) {
        counter(invoke(fun, cur));
        increment(cur);
    }
    transpose_op<Value_type<C>, Op> transposed_op(op);
    return reduce_nonzeroes(first(counter), limit(counter), transposed_op, zero);
}

template <Cursor C, Limit<C> L, Operation<Value_type<C>, Value_type<C>> Op>
requires Loadable<C>
constexpr auto
reduce_balanced(C cur, L lim, Op op, Value_type<C> const& zero) -> Value_type<C>
//[[expects axiom: loadable_range(cur, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    return reduce_balanced(mv(cur), mv(lim), op, [](C const& c){ return load(c); }, zero);
}

}
