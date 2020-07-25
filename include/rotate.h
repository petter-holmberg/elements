#pragma once

#include "integer.h"
#include "reverse.h"
#include "swap.h"

namespace elements {

template <Forward_cursor C, Limit<C> L>
requires Mutable<C>
constexpr void
rotate_step(C& cur0, C& cur1, L lim)
{
    auto cur = cur1;
    do {
        swap_step(cur0, cur);
        if (cur0 == cur1) cur1 = cur;
    } while (precedes(cur, lim));
}

template <Forward_cursor C, Limit<C> L>
requires Mutable<C>
constexpr auto
rotate_nontrivial(C cur0, L lim, C cur1) -> C
//[[expects: mutable_bounded_range(cur, lim)]]
//[[expects: precedes(cur0, cur1) and precedes(cur1, lim)]]
{
    rotate_step(cur0, cur1, lim);
    auto cur = cur0;
    while (precedes(cur1, lim)) rotate_step(cur0, cur1, lim);
    return cur;
}

template <Bidirectional_cursor C, Limit<C> L>
requires Mutable<C>
constexpr auto
rotate_nontrivial(C cur0, L lim, C cur1) -> C
//[[expects: mutable_bounded_range(cur, lim)]]
//[[expects: precedes(cur0, cur1) and precedes(cur1, lim)]]
{
    reverse(cur0, cur1);
    reverse(cur1, lim);
    auto [cur2, cur3] = reverse_swap(cur1, lim, cur0, cur1);
    reverse(cur3, cur2);
    if (cur1 == cur2) {
        return cur3;
    } else {
        return cur2;
    }
}

template <Forward_cursor C>
requires Mutable<C>
constexpr auto
rotate(C cur0, C lim, C cur1) -> C
//[[expects: mutable_bounded_range(cur, lim)]]
{
    if (cur1 == cur0) return lim;
    if (cur1 == lim) return cur0;
    return rotate_nontrivial(cur0, lim, cur1);
}

}
