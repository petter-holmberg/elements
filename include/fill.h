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

}