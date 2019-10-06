#pragma once

#include "binary_counter.h"

namespace elements {

template <Position P, Limit<P> L, Binary_operation Op, Unary_function Fun>
requires
    Loadable<P> and
    Same_as<P, Domain<Fun>> and
    Same_as<Domain<Op>, Codomain<Fun>>
constexpr auto
reduce_nonempty(P pos, L lim, Op op, Fun fun) -> Domain<Op>
//[[expects axiom: loadable_range(pos, lim)]]
//[[expects: precedes(pos, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    auto x = fun(pos);
    increment(pos);
    while (precedes(pos, lim)) {
        store(x, op(x, fun(pos)));
        increment(pos);
    }
    return x;
}

template <Position P, Limit<P> L, Binary_operation Op>
requires
    Loadable<P> and
    Same_as<Decay<Value_type<P>>, Domain<Op>>
constexpr auto
reduce_nonempty(P pos, L lim, Op op) -> Domain<Op>
//[[expects axiom: loadable_range(pos, lim)]]
//[[expects: precedes(pos, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    return reduce_nonempty(mv(pos), mv(lim), op, [](P const& x){ return load(x); });
}

template <Position P, Limit<P> L, Binary_operation Op, Unary_function Fun>
requires
    Loadable<P> and
    Same_as<P, Domain<Fun>> and
    Same_as<Domain<Op>, Codomain<Fun>>
constexpr auto
reduce(P pos, L lim, Op op, Fun fun, Domain<Op> const& zero) -> Domain<Op>
//[[expects axiom: loadable_range(pos, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    if (!precedes(pos, lim)) return zero;
    return reduce_nonempty(mv(pos), mv(lim), op, fun);
}

template <Position P, Limit<P> L, Binary_operation Op>
requires
    Loadable<P> and
    Same_as<Decay<Value_type<P>>, Domain<Op>>
constexpr auto
reduce(P pos, L lim, Op op, Domain<Op> const& zero) -> Domain<Op>
//[[expects axiom: loadable_range(pos, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    return reduce(mv(pos), mv(lim), op, [](P const& x){ return load(x); }, zero);
}

template <Position P, Limit<P> L, Binary_operation Op, Unary_function Fun>
requires
    Same_as<P, Domain<Fun>> and
    Same_as<Domain<Op>, Codomain<Fun>>
constexpr auto
reduce_nonzeroes(P pos, L lim, Op op, Fun fun, Domain<Op> const& zero) -> Domain<Op>
//[[expects axiom: loadable_range(pos, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    Domain<Op> x;
    do {
        if (!(precedes(pos, lim))) return zero;
        store(x, fun(pos));
        increment(pos);
    } while (x == zero);
    while (precedes(pos, lim)) {
        auto y = fun(pos);
        if (y != zero) store(x, op(x, y));
        increment(pos);
    }
    return x;
}

template <Position P, Limit<P> L, Binary_operation Op>
requires Same_as<Decay<Value_type<P>>, Domain<Op>>
constexpr auto
reduce_nonzeroes(P pos, L lim, Op op, Domain<Op> const& zero) -> Domain<Op>
//[[expects axiom: loadable_range(pos, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    return reduce_nonzeroes(mv(pos), mv(lim), op, [](P const& x){ return load(x); }, zero);
}

template <Position P, Limit<P> L, Binary_operation Op, Unary_function Fun>
requires
    Loadable<P> and
    Same_as<P, Domain<Fun>> and
    Same_as<Domain<Op>, Codomain<Fun>>
constexpr auto
reduce_balanced(P pos, L lim, Op op, Fun fun, Domain<Op> const& zero) -> Domain<Op>
//[[expects axiom: loadable_range(pos, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    binary_counter<Op> counter(op, zero);
    while (precedes(pos, lim)) {
        counter(fun(pos));
        increment(pos);
    }
    transpose_op<Op> transposed_op(op);
    return reduce_nonzeroes(first(counter), limit(counter), transposed_op, zero);
}

template <Position P, Limit<P> L, Binary_operation Op>
requires Loadable<P>
constexpr auto
reduce_balanced(P pos, L lim, Op op, Domain<Op> const& zero) -> Domain<Op>
//[[expects axiom: loadable_range(pos, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    return reduce_balanced(mv(pos), mv(lim), op, [](P const& x){ return load(x); }, zero);
}

}
