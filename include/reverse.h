#pragma once

#include "swap.h"

namespace elements {

template <Bidirectional_position P, Limit<P> L>
requires Mutable<P>
constexpr void
reverse(P pos, L lim)
//[[expects: mutable_bounded_range(pos, lim)]]
{
    while (true) {
        if (pos == lim) return;
        decrement(lim);
        if (pos == lim) return;
        swap(at(pos), at(lim));
        increment(pos);
    }
}

}
