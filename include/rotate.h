#pragma once

#include "integer.h"
#include "pair.h"
#include "reverse.h"
#include "swap.h"

#define concept concept bool // Use C++20 syntax

namespace elements {

template <Forward_position P, Limit<P> L>
requires Mutable<P>
constexpr void
rotate_step(P& pos0, P& pos1, L lim)
{
    auto pos = pos1;
    do {
        swap_step(pos0, pos);
        if (pos0 == pos1) pos1 = pos;
    } while (precedes(pos, lim));
}

template <Forward_position P, Limit<P> L>
requires Mutable<P>
constexpr auto
rotate_nontrivial(P pos0, L lim, P pos1) -> P
//[[expects: mutable_bounded_range(pos, lim)]]
//[[expects: precedes(pos0, pos1) and precedes(pos1, lim)]]
{
    rotate_step(pos0, pos1, lim);
    auto pos = pos0;
    while (precedes(pos1, lim)) rotate_step(pos0, pos1, lim);
    return pos;
}

template <Bidirectional_position P, Limit<P> L>
requires Mutable<P>
constexpr auto
rotate_nontrivial(P pos0, L lim, P pos1) -> P
//[[expects: mutable_bounded_range(pos, lim)]]
//[[expects: precedes(pos0, pos1) and precedes(pos1, lim)]]
{
    reverse(pos0, pos1);
    reverse(pos1, lim);
    auto [pos2, pos3] = reverse_swap(pos1, lim, pos0, pos1);
    reverse(pos3, pos2);
    if (pos1 == pos2) {
        return pos3;
    } else {
        return pos2;
    }
}

template <Forward_position P>
requires Mutable<P>
constexpr auto
rotate(P pos0, P lim, P pos1) -> P
//[[expects: mutable_bounded_range(pos, lim)]]
{
    if (pos1 == pos0) return lim;
    if (pos1 == lim) return pos0;
    return rotate_nontrivial(pos0, lim, pos1);
}

}
