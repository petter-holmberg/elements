#pragma once

#include "cursor.h"

namespace elements {

template <Cursor S, Cursor D>
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

template <Cursor S, Limit<S> L, Cursor D>
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

template <Segmented_cursor S, Cursor D>
requires
    Loadable<S> and
    Storable<D> and
    Same_as<Decay<Value_type<S>>, Decay<Value_type<D>>>
constexpr auto
copy(S src, S lim, D dst) -> D
{
    auto index_src = index_cursor(src);
    auto index_lim = index_cursor(lim);
    if (!precedes(index_src, index_lim)) {
        return copy(segment_cursor(src), segment_cursor(lim), dst);
    } else {
        dst = copy(segment_cursor(src), limit(load(index_src)), dst);
        do {
            increment(index_src);
            dst = copy(first(load(index_src)), limit(load(index_src)), dst);
        } while (precedes(index_src, index_lim));
        return dst;
    }
}

template <Cursor S, Cursor D, Limit<S> L, Predicate<S> P>
requires
    Loadable<S> and
    Storable<D>
constexpr auto
copy_select(S src, L lim, D dst, P pred) -> D
//[[expects axiom: not_overlapped_forward(src, lim, dst, dst + (# of iterators satisfying pred))]]
{
    auto cur{src};
    while (precedes(cur, lim)) {
        if (invoke(pred, cur)) {
            copy_step(cur, dst);
        } else {
            increment(cur);
        }
    }
    return dst;
}

template <Loadable L, Predicate<Value_type<L>> P>
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

template <Cursor S, Cursor D, Limit<S> L, Predicate<Value_type<S>> P>
requires
    Loadable<S> and
    Storable<D> and
    Same_as<Value_type<S>, Value_type<D>>
constexpr auto
copy_if(S src, L lim, D dst, P pred) -> D
//[[expects axiom: not_overlapped_forward(src, lim, dst, dst + (# of iterators satisfying pred))]]
{
    return copy_select(src, lim, dst, predicate_load<S, P>{pred});
}

template <Cursor S, Cursor D, Limit<S> L, Predicate<Value_type<S>> P>
requires
    Loadable<S> and
    Storable<D> and
    Same_as<Value_type<S>, Value_type<D>>
constexpr auto
copy_if_not(S src, L lim, D dst, P pred) -> D
//[[expects axiom: not_overlapped_forward(src, lim, dst, dst + (# of iterators satisfying pred))]]
{
    return copy_if(src, lim, dst, negation<Value_type<S>, P>{pred});
}

}
