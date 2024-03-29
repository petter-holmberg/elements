#pragma once

#include "for_each.h"

namespace elements {

template <typename T, Predicate<T> P, Cursor N>
struct predicate_counter
{
    P& pred;
    N& count;

    constexpr
    predicate_counter(P& pred_, N& count_)
        : pred{pred_}
        , count{count_}
    {}

    constexpr void
    operator()(T const& x)
    {
        if (invoke(pred, x)) increment(count);
    }
};

template <Cursor C, Limit<C> L, Predicate<Value_type<C>> P, Cursor N = Difference_type<C>>
requires Loadable<C>
constexpr auto
count_if(C cur, L lim, P pred, N count = Zero<N>) -> N
//[[expects axiom: loadable_range(cur, lim)]]
{
    return get<1>(for_each(mv(cur), lim, predicate_counter<Value_type<C>, P, N>{pred, count})).count;
}

template <Cursor C, Limit<C> L, Predicate<Value_type<C>> P, Cursor N = Difference_type<C>>
requires Loadable<C>
constexpr auto
count_if_not(C cur, L lim, P pred, N count = Zero<N>) -> N
//[[expects axiom: loadable_range(cur, lim)]]
{
    return count_if(mv(cur), lim, negation<Value_type<C>, P>{pred}, count);
}

template <Cursor C, Limit<C> L, Cursor N = Value_type<C>>
requires Loadable<C>
constexpr auto
count(C cur, L lim, Value_type<C> const& value, N count = Zero<N>) -> N
//[[expects axiom: loadable_range(cur, lim)]]
{
    return count_if(mv(cur), lim, eq_unary{value}, count);
}

template <Cursor C, Limit<C> L, Cursor N = Value_type<C>>
requires Loadable<C>
constexpr auto
count_not(C cur, L lim, Value_type<C> const& value, N count = Zero<N>) -> N
//[[expects axiom: loadable_range(cur, lim)]]
{
    return count_if_not(mv(cur), lim, eq_unary{value}, count);
}

}
