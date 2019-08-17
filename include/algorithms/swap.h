#pragma once

#include "position.h"
#include "regular.h"

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
    Same<Value_type<P0>, Value_type<P1>>
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
    Same<Value_type<P0>, Value_type<P1>>
constexpr auto
swap(P0 pos0, L lim, P1 pos1) -> P1
{
    while (precedes(pos0, lim)) swap_step(pos0, pos1);
    return pos1;
}

}
