#pragma once

#include "position.h"

namespace elements {

template <Position P, Limit<P> L, typename T>
requires Storable<P>
constexpr auto
fill(P pos, L lim, T const& value) -> P
//[[expects axiom: storable_range(pos, lim)]]
{
    while (precedes(pos, lim)) {
        store(pos, value);
        increment(pos);
    }
    return pos;
}

template <Segmented_position P, typename T>
requires Storable<P>
constexpr auto
fill(P pos, P lim, T const& value) -> P
{
    auto index_pos = index_pos(pos);
    auto index_lim = index_pos(lim);
    if (!precedes(index_pos, index_lim)) {
        return {index_pos, fill(segment_pos(pos), segment_pos(lim), value)};
    } else {
        auto segment_pos = fill(segment_pos(pos), limit(load(index_pos)), value);
        do {
            increment(index_pos);
            segment_pos = fill(first(load(index_pos)), limit(load(index_pos)), value);
        } while (precedes(index_pos, index_lim));
        return {index_pos, segment_pos};
    }
}

}
