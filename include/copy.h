#pragma once

#include "pair.h"

namespace elements {

template <Cursor S, Cursor D>
requires Indirectly_copyable<S, D>
constexpr void
copy_step(S& src, D& dst)
{
    store(dst, load(src));
    increment(dst);
    increment(src);
}

template <Cursor S, Limit<S> L, Cursor D>
requires Indirectly_copyable<S, D>
constexpr auto
copy(S src, L lim, D dst) -> D
//[[expects axiom: not_overlapped_forward(src, lim, dst, dst + (lim - src))]]
{
    while (precedes(src, lim)) copy_step(src, dst);
    return dst;
}

template <Cursor S, Limit<S> LS, Cursor D, Limit<D> LD>
requires Indirectly_copyable<S, D>
constexpr auto
copy(S src, LS lims, D dst, LD limd) -> pair<S, D>
//[[expects axiom: not_overlapped_forward(src, lims, dst, limd))]]
{
    while (precedes(src, lims) and precedes(dst, limd)) copy_step(src, dst);
    return {mv(src), mv(dst)};
}

template <Cursor S, Integer N, Cursor D>
requires Indirectly_copyable<S, D>
constexpr auto
copy_n(S src, N n, D dst) -> pair<S, D>
{
   while (count_down(n)) copy_step(src, dst);
   return {mv(src), mv(dst)};
}

template <Segmented_cursor S, Cursor D>
requires Indirectly_copyable<S, D>
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
requires Indirectly_copyable<S, D>
constexpr auto
copy_select(S src, L lim, D dst, P pred) -> D
//[[expects axiom: not_overlapped_forward(src, lim, dst, dst + (# of cursors satisfying pred))]]
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
requires Indirectly_copyable<S, D>
constexpr auto
copy_if(S src, L lim, D dst, P pred) -> D
//[[expects axiom: not_overlapped_forward(src, lim, dst, dst + (# of cursors satisfying pred))]]
{
    return copy_select(mv(src), lim, mv(dst), predicate_load<S, P>{pred});
}

template <Cursor S, Cursor D, Limit<S> L, Predicate<Value_type<S>> P>
requires Indirectly_copyable<S, D>
constexpr auto
copy_if_not(S src, L lim, D dst, P pred) -> D
//[[expects axiom: not_overlapped_forward(src, lim, dst, dst + (# of cursors satisfying pred))]]
{
    return copy_if(mv(src), lim, mv(dst), negation<Value_type<S>, P>{pred});
}

}
