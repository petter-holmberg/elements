#pragma once

#include "pair.h"

namespace elements {

template <Movable T>
constexpr void
swap(T& x, T& y)
{
    auto z = mv(x);
    x = mv(y);
    y = mv(z);
}

template <Forward_position P0, Forward_position P1>
requires
    Mutable<P0> and
    Mutable<P1> and
    Same_as<Value_type<P0>, Value_type<P1>>
constexpr void
swap_step(P0& pos0, P1& pos1)
{
    swap(at(pos0), at(pos1));
    increment(pos0);
    increment(pos1);
}

template <Forward_position P0, Limit<P0> L, Forward_position P1>
requires
    Mutable<P0> and
    Mutable<P1> and
    Same_as<Value_type<P0>, Value_type<P1>>
constexpr auto
swap(P0 pos0, L lim, P1 pos1) -> P1
{
    while (precedes(pos0, lim)) swap_step(pos0, pos1);
    return pos1;
}

template <Bidirectional_position P0, Forward_position P1>
requires
    Mutable<P0> and
    Mutable<P1> and
    Same_as<Value_type<P0>, Value_type<P1>>
constexpr void
reverse_swap_step(P0& pos0, P1& pos1)
{
    decrement(pos0);
    swap(at(pos0), at(pos1));
    increment(pos1);
}

template <Bidirectional_position P0, Limit<P0> L0, Bidirectional_position P1, Limit<P1> L1>
requires
    Mutable<P0> and
    Mutable<P1>
constexpr auto
reverse_swap(P0 pos0, L0 lim0, P1 pos1, L1 lim1) -> pair<P0, P1>
//[[expects: mutable_bounded_range(pos0, lim0)]]
//[[expects: mutable_bounded_range(pos1, lim1)]]
{
    while (precedes(pos0, lim0) and precedes(pos1, lim1)) reverse_swap_step(lim0, pos1);
    return {lim0, pos1};
}

}
