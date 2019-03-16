#pragma once

#include "invocable.h"
#include "integer.h"
#include "position.h"

namespace elements {

template <Loadable_position P, Limit<P> L, Binary_operation Op, Unary_function Fun>
requires
    Same<P, Domain<Fun>> and
    Same<Domain<Op>, Codomain<Fun>>
constexpr auto
reduce_nonempty(P pos, L lim, Op op, Fun fun) -> Domain<Op>
//[[expects axiom: loadable_range(pos, lim)]]
//[[expects: precedes(pos, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    auto x = fun(pos);
    increment(pos);
    while (precedes(pos, lim)) {
        x = op(x, fun(pos));
        increment(pos);
    }
    return x;
}

template <Loadable_position P, Limit<P> L, Binary_operation Op>
requires Same<Decay<Value_type<P>>, Domain<Op>>
constexpr auto
reduce_nonempty(P pos, L lim, Op op) -> Domain<Op>
//[[expects axiom: loadable_range(pos, lim)]]
//[[expects: precedes(pos, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    return reduce_nonempty(pos, lim, op, [](P const& x){ return load(x); });
}

template <Loadable_position P, Limit<P> L, Binary_operation Op, Unary_function Fun>
requires
    Same<P, Domain<Fun>> and
    Same<Domain<Op>, Codomain<Fun>>
constexpr auto
reduce(P pos, L lim, Op op, Fun fun, Domain<Op> const& zero) -> Domain<Op>
//[[expects axiom: loadable_range(pos, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    if (!precedes(pos, lim)) return zero;
    return reduce_nonempty(pos, lim, op, fun);
}

template <Loadable_position P, Limit<P> L, Binary_operation Op>
requires Same<Decay<Value_type<P>>, Domain<Op>>
constexpr auto
reduce(P pos, L lim, Op op, Domain<Op> const& zero) -> Domain<Op>
//[[expects axiom: loadable_range(pos, lim)]]
//[[expects axiom: partially_associative(op)]]
{
    if (!precedes(pos, lim)) return zero;
    return reduce_nonempty(pos, lim, op);
}

}
