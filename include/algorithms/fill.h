#pragma once

#include "concepts/position.h"

namespace elements {

template <Storable_position P, Limit<P> L, typename T>
constexpr void
fill(P pos, L lim, T const& value)
//[[expects axiom: storable_range(pos, lim)]]
{
    while (pos != lim) {
        store(pos, value);
        increment(pos);
    }
}

}
