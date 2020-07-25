#pragma once

#include "binary_counter.h"

namespace elements {

template <Cursor C, Limit<C> L, Binary_operation Op, Unary_function F>
requires
    Loadable<C> and
    Same_as<C, Domain<F>> and
    Same_as<Domain<Op>, Codomain<F>>
constexpr auto
reduce_nonempty(C cur, L lim, Op op, F fun) -> Domain<Op>
//[[expects axiom: loadable_range(cur, lim)]]
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

template <Cursor C, Limit<C> L, Binary_operation Op>
requires
    Loadable<C> and
    Same_as<Decay<Value_type<C>>, Domain<Op>>
constexpr auto
reduce_nonempty(C cur, L lim, Op op) -> Domain<Op>
//[[expects axiom: loadable_range(cur, lim)]]
//[[expects: precedes(cur, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    return reduce_nonempty(mv(cur), mv(lim), op, [](C const& c){ return load(c); });
}

template <Cursor C, Limit<C> L, Binary_operation Op, Unary_function F>
requires
    Loadable<C> and
    Same_as<C, Domain<F>> and
    Same_as<Domain<Op>, Codomain<F>>
constexpr auto
reduce(C cur, L lim, Op op, F fun, Domain<Op> const& zero) -> Domain<Op>
//[[expects axiom: loadable_range(cur, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    if (!precedes(cur, lim)) return zero;
    return reduce_nonempty(mv(cur), mv(lim), op, fun);
}

template <Cursor C, Limit<C> L, Binary_operation Op>
requires
    Loadable<C> and
    Same_as<Decay<Value_type<C>>, Domain<Op>>
constexpr auto
reduce(C cur, L lim, Op op, Domain<Op> const& zero) -> Domain<Op>
//[[expects axiom: loadable_range(cur, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    return reduce(mv(cur), mv(lim), op, [](C const& c){ return load(c); }, zero);
}

template <Cursor C, Limit<C> L, Binary_operation Op, Unary_function F>
requires
    Same_as<C, Domain<F>> and
    Same_as<Domain<Op>, Codomain<F>>
constexpr auto
reduce_nonzeroes(C cur, L lim, Op op, F fun, Domain<Op> const& zero) -> Domain<Op>
//[[expects axiom: loadable_range(cur, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    Domain<Op> x;
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

template <Cursor C, Limit<C> L, Binary_operation Op>
requires Same_as<Decay<Value_type<C>>, Domain<Op>>
constexpr auto
reduce_nonzeroes(C cur, L lim, Op op, Domain<Op> const& zero) -> Domain<Op>
//[[expects axiom: loadable_range(cur, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    return reduce_nonzeroes(mv(cur), mv(lim), op, [](C const& c){ return load(c); }, zero);
}

template <Cursor C, Limit<C> L, Binary_operation Op, Unary_function F>
requires
    Loadable<C> and
    Same_as<C, Domain<F>> and
    Same_as<Domain<Op>, Codomain<F>>
constexpr auto
reduce_balanced(C cur, L lim, Op op, F fun, Domain<Op> const& zero) -> Domain<Op>
//[[expects axiom: loadable_range(cur, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    binary_counter<Op> counter(op, zero);
    while (precedes(cur, lim)) {
        counter(invoke(fun, cur));
        increment(cur);
    }
    transpose_op<Op> transposed_op(op);
    return reduce_nonzeroes(first(counter), limit(counter), transposed_op, zero);
}

template <Cursor C, Limit<C> L, Binary_operation Op>
requires Loadable<C>
constexpr auto
reduce_balanced(C cur, L lim, Op op, Domain<Op> const& zero) -> Domain<Op>
//[[expects axiom: loadable_range(cur, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    return reduce_balanced(mv(cur), mv(lim), op, [](C const& c){ return load(c); }, zero);
}

}
