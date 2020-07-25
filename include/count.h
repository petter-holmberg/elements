#pragma once

#include "for_each.h"

namespace elements {

template <Unary_predicate P, Cursor N>
struct counter
{
    P& pred;
    N& count;

    constexpr
    counter(P& pred_, N& count_)
        : pred{pred_}
        , count{count_}
    {}

    constexpr void
    operator()(Domain<P> const& x)
    {
        if (invoke(pred, x)) increment(count);
    }
};

template <Cursor C, Limit<C> L, Unary_predicate P, Cursor N = Difference_type<C>>
requires
    Loadable<C> and
    Same_as<Value_type<C>, Domain<P>>
constexpr auto
count_if(C cur, L lim, P pred, N count = Zero<N>) -> N
//[[expects axiom: loadable_range(cur, lim)]]
{
    return for_each(mv(cur), lim, counter{pred, count}).procedure.count;
}

template <Cursor C, Limit<C> L, Unary_predicate P, Cursor N = Difference_type<C>>
requires
    Loadable<C> and
    Same_as<Value_type<C>, Domain<P>>
constexpr auto
count_if_not(C cur, L lim, P pred, N count = Zero<N>) -> N
//[[expects axiom: loadable_range(cur, lim)]]
{
    return count_if(mv(cur), lim, negation{pred}, count);
}

template <Cursor C, Limit<C> L, Cursor N = Value_type<C>>
requires Loadable<C>
constexpr auto
count(C cur, L lim, Value_type<C> const& value, N count = Zero<N>) -> N
//[[expects axiom: loadable_range(cur, lim)]]
{
    return count_if(mv(cur), lim, equal_unary{value}, count);
}

template <Cursor C, Limit<C> L, Cursor N = Value_type<C>>
requires Loadable<C>
constexpr auto
count_not(C cur, L lim, Value_type<C> const& value, N count = Zero<N>) -> N
//[[expects axiom: loadable_range(cur, lim)]]
{
    return count_if_not(mv(cur), lim, equal_unary{value}, count);
}

}
