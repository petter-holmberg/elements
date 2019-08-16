#pragma once

#include "invocable.h"
#include "position.h"
#include "regular.h"
#include "for_each.h"

namespace elements {

template <Unary_predicate U, Position P>
struct counter
{
    U& pred;
    P& count;

    constexpr
    counter(U& pred_, P& count_)
        : pred{pred_}
        , count{count_}
    {}

    constexpr void
    operator()(Domain<U> const& x)
    {
        if (pred(x)) increment(count);
    }
};

template <Position P, Limit<P> L, Unary_predicate U, Position C = Difference_type<P>>
requires
    Loadable<P> and
    Same<Value_type<P>, Domain<U>>
constexpr auto
count_if(P pos, L lim, U pred, C count = Zero<C>) -> C
//[[expects axiom: loadable_range(pos, lim)]]
{
    return for_each(mv(pos), lim, counter{pred, count}).count;
}

template <Position P, Limit<P> L, Unary_predicate U, Position C = Difference_type<P>>
requires
    Loadable<P> and
    Same<Value_type<P>, Domain<U>>
constexpr auto
count_if_not(P pos, L lim, U pred, C count = Zero<C>) -> C
//[[expects axiom: loadable_range(pos, lim)]]
{
    return count_if(mv(pos), lim, negation{pred}, count);
}

template <Position P, Limit<P> L, Position C = Value_type<P>>
requires Loadable<P>
constexpr auto
count(P pos, L lim, Value_type<P> const& value, C count = Zero<C>) -> C
//[[expects axiom: loadable_range(pos, lim)]]
{
    return count_if(mv(pos), lim, equal_unary{value}, count);
}

template <Position P, Limit<P> L, Position C = Value_type<P>>
requires Loadable<P>
constexpr auto
count_not(P pos, L lim, Value_type<P> const& value, C count = Zero<C>) -> C
//[[expects axiom: loadable_range(pos, lim)]]
{
    return count_if_not(mv(pos), lim, equal_unary{value}, count);
}

}
