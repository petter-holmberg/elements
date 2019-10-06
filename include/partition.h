#pragma once

#include "search.h"

namespace elements {

template <Position P, Limit<P> L, Unary_predicate U>
requires
    Loadable<P> and
    Same_as<Value_type<P>, Domain<U>>
constexpr auto
is_partitioned(P pos, L lim, U pred) -> bool
//[[expects axiom: loadable_range(pos, lim)]]
{
    return !precedes(search_if_not(search_if(mv(pos), lim, pred), lim, pred), lim);
}

template <Forward_position P, Unary_predicate U>
requires
    Loadable<P> and
    Same_as<Value_type<P>, Domain<U>>
constexpr auto
partition_point_counted(P pos, Difference_type<P> dist, U pred) -> P
//[[expects axiom: loadable_counted_range(pos, dist)]]
//[[expects: is_partitioned(counted_position(pos, dist), pred)]]
{
    while (!is_zero(dist))
    {
        auto const half_dist = half(dist);
        auto const mid = pos + half_dist;
        if (pred(load(mid))) {
            dist = half_dist;
        } else {
            dist = dist - successor(half_dist);
            pos = successor(mid);
        }
    }
    return pos;
}

template <Forward_position P, Limit<P> L, Unary_predicate U>
requires
    Loadable<P> and
    Same_as<Value_type<P>, Domain<U>>
constexpr auto
partition_point(P pos, L lim, U pred) -> P
//[[expects axiom: loadable_range(pos, lim)]]
//[[expects: is_partitioned(pos, lim, pred)]]
{
    return partition_point_counted(pos, lim - pos, pred);
}

}
