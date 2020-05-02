#pragma once

#include "position.h"

namespace elements {

template <Position P, Limit<P> L, Movable T>
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

template <Segmented_position P, Movable T>
requires Storable<P>
constexpr auto
fill(P pos, P lim, T const& value) -> P
{
    auto index_pos_ = index_pos(pos);
    auto index_lim_ = index_pos(lim);
    if (!precedes(index_pos_, index_lim_)) {
        return {index_pos_, fill(segment_pos(pos), segment_pos(lim), value)};
    } else {
        auto segment_pos_ = fill(segment_pos(pos), limit(load(index_pos_)), value);
        do {
            increment(index_pos_);
            segment_pos_ = fill(first(load(index_pos_)), limit(load(index_pos_)), value);
        } while (precedes(index_pos_, index_lim_));
        return {index_pos_, segment_pos_};
    }
}

}
