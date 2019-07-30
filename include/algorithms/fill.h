#pragma once

#include "concepts/position.h"

namespace elements {

template <Position P, Limit<P> L, typename T>
requires Storable<P>
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
