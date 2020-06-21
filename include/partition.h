#pragma once

#include "copy.h"
#include "pair.h"
#include "search.h"
#include "swap.h"

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
        if (invoke(pred, load(mid))) {
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

template <Forward_position P, Limit<P> L, Unary_predicate U>
requires
    Mutable<P> and
    Same_as<Value_type<P>, Domain<U>>
constexpr auto
partition_semistable(P pos, L lim, U pred) -> P
{
    while (true) {
        if (!precedes(pos, lim)) return pos;
        if (invoke(pred, load(pos))) break;
        increment(pos);
    }
    auto partition_point = pos;
    increment(pos);
    while (true) {
        while (true) {
            if (!precedes(pos, lim)) return partition_point;
            if (!invoke(pred, load(pos))) break;
            increment(pos);
        }
        swap_step(partition_point, pos);
    }
}

template <Loadable L, Predicate P>
requires Same_as<Value_type<L>, Domain<P>>
struct predicate_source
{
    P pred;

    explicit
    predicate_source(P const& pred_) : pred(pred_) {}

    constexpr auto
    operator()(L const& x) -> bool
    {
        return pred(load(x));
    }
};

template <Position S, Limit<S> L, Position D0, Position D1, Unary_predicate U>
requires
    Loadable<S> and
    Storable<D0> and
    Storable<D1> and
    Same_as<Value_type<S>, Value_type<D0>> and
    Same_as<Value_type<S>, Value_type<D1>> and
    Unary_predicate<U> and
    Same_as<S, Domain<U>>
constexpr auto
split_copy(S src, L lim, D0 dst_false, D1 dst_true, U pred) -> pair<D0, D1>
{
    while (precedes(src, lim)) {
        if (pred(src)) {
            copy_step(src, dst_true);
        } else {
            copy_step(src, dst_false);
        }
    }
    return {dst_false, dst_true};
}

template <Position S, Limit<S> L, Position D0, Position D1, Unary_predicate U>
requires
    Loadable<S> and
    Storable<D0> and
    Storable<D1> and
    Same_as<Value_type<S>, Value_type<D0>> and
    Same_as<Value_type<S>, Value_type<D1>> and
    Unary_predicate<U> and
    Same_as<Value_type<S>, Domain<U>>
constexpr auto
partition_copy(S src, L lim, D0 dst_false, D1 dst_true, U pred) -> pair<D0, D1>
{
    predicate_source<S, U> pred_src{pred};
    return split_copy(src, lim, dst_false, dst_true, pred_src);
}

template <Forward_position P, Limit<P> L, Forward_position B, Unary_predicate U>
requires
    Mutable<P> and
    Mutable<B> and
    Same_as<Value_type<P>, Value_type<B>> and
    Same_as<Value_type<P>, Domain<U>>
constexpr auto
partition_with_buffer(P pos, L lim, B buf, U pred) -> P
//[[expects axiom: mutable_range(pos, lim)]]
//[[expects axiom: mutable_range(buf, distance(pos, lim))]]
{
    auto partition_points = partition_copy(pos, lim, pos, buf, pred);
    copy(buf, partition_points.m1, partition_points.m0);
    return partition_points.m0;
}

template <Bidirectional_position P, Limit<P> L, Unary_predicate U>
requires
    Mutable<P> and
    Same_as<Value_type<P>, Domain<U>>
constexpr auto
partition(P pos, L lim, U pred) -> P
//[[expects axiom: mutable_range(pos, lim)]]
{
    while (true) {
        pos = search_if(pos, lim, pred);
        auto swap_pos = search_backward_if_not(pos, lim, pred);
        if (pos == swap_pos) return pos;
        reverse_swap_step(swap_pos, pos);
    }
}

}
