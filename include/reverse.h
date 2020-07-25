#pragma once

#include "swap.h"

namespace elements {

template <Bidirectional_cursor C, Limit<C> L>
requires Mutable<C>
constexpr void
reverse(C cur, L lim)
//[[expects: mutable_bounded_range(cur, lim)]]
{
    while (true) {
        if (cur == lim) return;
        decrement(lim);
        if (cur == lim) return;
        swap(at(cur), at(lim));
        increment(cur);
    }
}

}
