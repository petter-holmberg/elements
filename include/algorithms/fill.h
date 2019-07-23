#pragma once

#include "concepts/position.h"

namespace elements {

template <typename P, Limit<P> L, typename T>
requires
    Storable<P> and Position<P>
constexpr void
fill(P pos, L lim, T const& value)
//[[expects axiom: storable_range(pos, lim)]]
{
    while (precedes(pos, lim)) {
        store(pos, value);
        increment(pos);
    }
}

}
