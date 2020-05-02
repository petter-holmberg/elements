#pragma once

#include "position.h"

namespace elements {

template <Position S, Position D>
requires
    Loadable<S> and
    Storable<D> and
    Same_as<Decay<Value_type<S>>, Decay<Value_type<D>>>
constexpr void
copy_step(S& src, D& dst)
{
    store(dst, load(src));
    increment(dst);
    increment(src);
}

template <Position S, Limit<S> L, Position D>
requires
    Loadable<S> and
    Storable<D> and
    Same_as<Decay<Value_type<S>>, Decay<Value_type<D>>>
constexpr auto
copy(S src, L lim, D dst) -> D
//[[expects axiom: not_overlapped_forward(src, lim, dst, dst + (lim - src))]]
{
    while (precedes(src, lim)) copy_step(src, dst);
    return dst;
}

template <Segmented_position S, Position D>
requires
    Loadable<S> and
    Storable<D> and
    Same_as<Decay<Value_type<S>>, Decay<Value_type<D>>>
constexpr auto
copy(S src, S lim, D dst) -> D
{
    auto index_src = index_pos(src);
    auto index_lim = index_pos(lim);
    if (!precedes(index_src, index_lim)) {
        return copy(segment_pos(src), segment_pos(lim), dst);
    } else {
        dst = copy(segment_pos(src), limit(load(index_src)), dst);
        do {
            increment(index_src);
            dst = copy(first(load(index_src)), limit(load(index_src)), dst);
        } while (precedes(index_src, index_lim));
        return dst;
    }
}

template <Position S, Position D, Limit<S> L, Unary_predicate P>
requires
    Loadable<S> and
    Storable<D> and
    Same_as<Value_type<S>, Value_type<D>> and
    Same_as<S, Domain<P>>
constexpr auto
copy_select(S src, L lim, D dst, P pred) -> D
//[[expects axiom: not_overlapped_forward(src, lim, dst, dst + (# of iterators satisfying pred))]]
{
    auto pos{src};
    while (precedes(pos, lim)) {
        if (invoke(pred, pos)) {
            copy_step(pos, dst);
        } else {
            increment(pos);
        }
    }
    return dst;
}

template <Loadable L, Unary_predicate P>
requires Same_as<Value_type<L>, Domain<P>>
struct predicate_load
{
    P pred;

    constexpr
    predicate_load(P const& pred_)
        : pred{pred_}
    {}

    constexpr auto
    operator()(L const& x) -> bool
    {
        return invoke(pred, load(x));
    }
};

template <Position S, Position D, Limit<S> L, Unary_predicate P>
requires
    Loadable<S> and
    Storable<D> and
    Same_as<Value_type<S>, Value_type<D>> and
    Same_as<Value_type<S>, Domain<P>>
constexpr auto
copy_if(S src, L lim, D dst, P pred) -> D
//[[expects axiom: not_overlapped_forward(src, lim, dst, dst + (# of iterators satisfying pred))]]
{
    return copy_select(src, lim, dst, predicate_load<S, P>{pred});
}

template <Position S, Position D, Limit<S> L, Unary_predicate P>
requires
    Loadable<S> and
    Storable<D> and
    Same_as<Value_type<S>, Value_type<D>> and
    Same_as<Value_type<S>, Domain<P>>
constexpr auto
copy_if_not(S src, L lim, D dst, P pred) -> D
//[[expects axiom: not_overlapped_forward(src, lim, dst, dst + (# of iterators satisfying pred))]]
{
    return copy_if(src, lim, dst, negation{pred});
}

}
