#pragma once

#include "copy.h"
#include "pair.h"
#include "search.h"
#include "swap.h"

namespace elements {

template <Cursor C, Limit<C> L, Unary_predicate P>
requires
    Loadable<C> and
    Same_as<Value_type<C>, Domain<P>>
constexpr auto
is_partitioned(C cur, L lim, P pred) -> bool
//[[expects axiom: loadable_range(cur, lim)]]
{
    return !precedes(search_if_not(search_if(mv(cur), lim, pred), lim, pred), lim);
}

template <Forward_cursor C, Unary_predicate P>
requires
    Loadable<C> and
    Same_as<Value_type<C>, Domain<P>>
constexpr auto
partition_point_counted(C cur, Difference_type<C> dist, P pred) -> C
//[[expects axiom: loadable_counted_range(cur, dist)]]
//[[expects: is_partitioned(counted_cursor(cur, dist), pred)]]
{
    while (!is_zero(dist))
    {
        auto const half_dist = half(dist);
        auto const mid = cur + half_dist;
        if (invoke(pred, load(mid))) {
            dist = half_dist;
        } else {
            dist = dist - successor(half_dist);
            cur = successor(mid);
        }
    }
    return cur;
}

template <Forward_cursor C, Limit<C> L, Unary_predicate P>
requires
    Loadable<C> and
    Same_as<Value_type<C>, Domain<P>>
constexpr auto
partition_point(C cur, L lim, P pred) -> C
//[[expects axiom: loadable_range(cur, lim)]]
//[[expects: is_partitioned(cur, lim, pred)]]
{
    return partition_point_counted(cur, lim - cur, pred);
}

template <Forward_cursor C, Limit<C> L, Unary_predicate P>
requires
    Mutable<C> and
    Same_as<Value_type<C>, Domain<P>>
constexpr auto
partition_semistable(C cur, L lim, P pred) -> C
{
    while (true) {
        if (!precedes(cur, lim)) return cur;
        if (invoke(pred, load(cur))) break;
        increment(cur);
    }
    auto partition_point = cur;
    increment(cur);
    while (true) {
        while (true) {
            if (!precedes(cur, lim)) return partition_point;
            if (!invoke(pred, load(cur))) break;
            increment(cur);
        }
        swap_step(partition_point, cur);
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

template <Cursor S, Limit<S> L, Cursor D0, Cursor D1, Unary_predicate P>
requires
    Loadable<S> and
    Storable<D0> and
    Storable<D1> and
    Same_as<Value_type<S>, Value_type<D0>> and
    Same_as<Value_type<S>, Value_type<D1>> and
    Unary_predicate<P> and
    Same_as<S, Domain<P>>
constexpr auto
split_copy(S src, L lim, D0 dst_false, D1 dst_true, P pred) -> pair<D0, D1>
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

template <Cursor S, Limit<S> L, Cursor D0, Cursor D1, Unary_predicate P>
requires
    Loadable<S> and
    Storable<D0> and
    Storable<D1> and
    Same_as<Value_type<S>, Value_type<D0>> and
    Same_as<Value_type<S>, Value_type<D1>> and
    Unary_predicate<P> and
    Same_as<Value_type<S>, Domain<P>>
constexpr auto
partition_copy(S src, L lim, D0 dst_false, D1 dst_true, P pred) -> pair<D0, D1>
{
    predicate_source<S, P> pred_src{pred};
    return split_copy(src, lim, dst_false, dst_true, pred_src);
}

template <Forward_cursor C, Limit<C> L, Forward_cursor B, Unary_predicate P>
requires
    Mutable<C> and
    Mutable<B> and
    Same_as<Value_type<C>, Value_type<B>> and
    Same_as<Value_type<C>, Domain<P>>
constexpr auto
partition_with_buffer(C cur, L lim, B buf, P pred) -> C
//[[expects axiom: mutable_range(cur, lim)]]
//[[expects axiom: mutable_range(buf, distance(cur, lim))]]
{
    auto partition_points = partition_copy(cur, lim, cur, buf, pred);
    copy(buf, partition_points.m1, partition_points.m0);
    return partition_points.m0;
}

template <Bidirectional_cursor C, Limit<C> L, Unary_predicate P>
requires
    Mutable<C> and
    Same_as<Value_type<C>, Domain<P>>
constexpr auto
partition(C cur, L lim, P pred) -> C
//[[expects axiom: mutable_range(cur, lim)]]
{
    while (true) {
        cur = search_if(cur, lim, pred);
        auto swap_cur = search_backward_if_not(cur, lim, pred);
        if (cur == swap_cur) return cur;
        reverse_swap_step(swap_cur, cur);
    }
}

}
