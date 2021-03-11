#pragma once

#include "partition.h"

namespace elements {

template <Forward_cursor C, Limit<C> L, Forward_cursor B, Predicate<Value_type<C>> P>
requires
    Mutable<C> and
    Mutable<B> and
    Same_as<Value_type<C>, Value_type<B>>
constexpr auto
gather_stable_with_buffer(C cur0, L lim, C cur1, B buf, P pred) -> bounded_range<C>
//[[expects axiom: mutable_range(cur0, lim)]]
//[[expects axiom: precedes(cur0, cur1)]]
//[[expects axiom: precedes(cur1, lim)]]
//[[expects axiom: mutable_range(buf, distance(cur0, lim))]]
{
    return {partition_stable_with_buffer(cur0, cur1, buf, pred), partition_stable_with_buffer(cur1, lim, buf, negation<Value_type<C>, P>{pred})};
}

template <Forward_cursor C, Limit<C> L, Predicate<Value_type<C>> P>
requires Mutable<C>
constexpr auto
gather_stable(C cur0, L lim, C cur1, P pred) -> bounded_range<C>
//[[expects axiom: mutable_range(cur0, lim)]]
//[[expects axiom: precedes(cur0, cur1)]]
//[[expects axiom: precedes(cur1, lim)]]
{
    return {partition_stable(cur0, cur1, pred), partition_stable(cur1, lim, negation<Value_type<C>, P>{pred})};
}

template <Forward_cursor C, Limit<C> L, Predicate<Value_type<C>> P>
requires Mutable<C>
constexpr auto
gather_unstable(C cur0, L lim, C cur1, P pred) -> bounded_range<C>
//[[expects axiom: mutable_range(cur0, lim)]]
//[[expects axiom: precedes(cur0, cur1)]]
//[[expects axiom: precedes(cur1, lim)]]
{
    return {partition_semistable(cur0, cur1, pred), partition_semistable(cur1, lim, negation<Value_type<C>, P>{pred})};
}

template <Bidirectional_cursor C, Limit<C> L, Predicate<Value_type<C>> P>
requires Mutable<C>
constexpr auto
gather_unstable(C cur0, L lim, C cur1, P pred) -> bounded_range<C>
//[[expects axiom: mutable_range(cur0, lim)]]
//[[expects axiom: precedes(cur0, cur1)]]
//[[expects axiom: precedes(cur1, lim)]]
{
    return {partition_unstable(cur0, cur1, pred), partition_unstable(cur1, lim, negation<Value_type<C>, P>{pred})};
}

}
