#pragma once

#include "cursor.h"

namespace elements {

template <Cursor C, Limit<C> L, Movable T>
requires Storable<C>
constexpr auto
fill(C cur, L lim, T const& value) -> C
//[[expects axiom: storable_range(cur, lim)]]
{
    while (precedes(cur, lim)) {
        store(cur, value);
        increment(cur);
    }
    return cur;
}

template <Cursor C, Limit<C> L, Movable T, Invocable<C, T> S>
constexpr auto
fill(C cur, L lim, T const& value, S sink) -> C
{
    while (precedes(cur, lim)) {
        invoke(sink, cur, value);
        increment(cur);
    }
    return cur;
}

template <Segmented_cursor C, Limit<C> L, Movable T>
requires Storable<C>
constexpr auto
fill(C cur, L lim, T const& value) -> C
{
    auto index_cur = index_cursor(cur);
    auto index_lim = index_cursor(lim);
    if (!precedes(index_cur, index_lim)) {
        return {index_cur, fill(segment_cursor(cur), segment_cursor(lim), value)};
    } else {
        auto segment_cur = fill(segment_cursor(cur), limit(load(index_cur)), value);
        do {
            increment(index_cur);
            segment_cur = fill(first(load(index_cur)), limit(load(index_cur)), value);
        } while (precedes(index_cur, index_lim));
        return {index_cur, segment_cur};
    }
}

template <Segmented_cursor C, Limit<C> L, Movable T, Invocable<C, T> S>
constexpr auto
fill(C cur, L lim, T const& value, S sink) -> C
{
    auto index_cur = index_cursor(cur);
    auto index_lim = index_cursor(lim);
    if (!precedes(index_cur, index_lim)) {
        return {index_cur, fill(segment_cursor(cur), segment_cursor(lim), value, sink)};
    } else {
        auto segment_cur = fill(segment_cursor(cur), limit(load(index_cur)), value, sink);
        do {
            increment(index_cur);
            segment_cur = fill(first(load(index_cur)), limit(load(index_cur)), value, sink);
        } while (precedes(index_cur, index_lim));
        return {index_cur, segment_cur};
    }
}

}
