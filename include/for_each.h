#pragma once

#include "pair.h"

namespace elements {

template <Cursor C, Limit<C> L, Invocable<Value_type<C>> P>
requires Loadable<C>
constexpr auto
for_each(C cur, L lim, P proc) -> pair<C, P>
//[[expects axiom: loadable_range(cur, lim)]]
{
    while (precedes(cur, lim)) {
        invoke(proc, load(cur));
        increment(cur);
    }
    return {mv(cur), mv(proc)};
}

template <Segmented_cursor C, Invocable<Value_type<C>> P>
requires Loadable<C>
constexpr auto
for_each(C cur, C lim, P proc) -> pair<C, P>
{
    auto index_cur = index_cursor(cur);
    auto index_lim = index_cursor(lim);
    if (!precedes(index_cur, index_lim)) {
        return for_each(segment_cursor(cur), segment_cursor(lim), proc);
    } else {
        proc = for_each(segment_cursor(cur), limit(load(index_cur)), proc);
        do {
            increment(index_cur);
            proc = for_each(first(load(index_cur)), limit(load(index_cur)), proc);
        } while (precedes(index_cur, index_lim));
        return {mv(cur), mv(proc)};
    }
}

template <Cursor C, Invocable<Value_type<C>> P>
requires Loadable<C>
constexpr auto
for_each_n(C cur, Difference_type<C> n, P proc) -> pair<C, P>
//[[expects axiom: is_weak_range(cur, n)]]
{
    while (count_down(n)) {
        invoke(proc, load(cur));
        increment(cur);
    }
    return {mv(cur), mv(proc)};
}

}
