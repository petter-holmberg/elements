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

template <Forward_cursor C0, Forward_cursor C1>
requires
    Mutable<C0> and
    Mutable<C1> and
    Same_as<Value_type<C0>, Value_type<C1>>
constexpr void
swap_step(C0& cur0, C1& cur1)
{
    swap(at(cur0), at(cur1));
    increment(cur0);
    increment(cur1);
}

template <Forward_cursor C0, Limit<C0> L, Forward_cursor C1>
requires
    Mutable<C0> and
    Mutable<C1> and
    Same_as<Value_type<C0>, Value_type<C1>>
constexpr auto
swap(C0 cur0, L lim, C1 cur1) -> C1
{
    while (precedes(cur0, lim)) swap_step(cur0, cur1);
    return cur1;
}

template <Bidirectional_cursor C0, Forward_cursor C1>
requires
    Mutable<C0> and
    Mutable<C1> and
    Same_as<Value_type<C0>, Value_type<C1>>
constexpr void
reverse_swap_step(C0& cur0, C1& cur1)
{
    decrement(cur0);
    swap(at(cur0), at(cur1));
    increment(cur1);
}

template <Bidirectional_cursor C0, Limit<C0> L0, Bidirectional_cursor C1, Limit<C1> L1>
requires
    Mutable<C0> and
    Mutable<C1>
constexpr auto
reverse_swap(C0 cur0, L0 lim0, C1 cur1, L1 lim1) -> pair<C0, C1>
//[[expects: mutable_bounded_range(cur0, lim0)]]
//[[expects: mutable_bounded_range(cur1, lim1)]]
{
    while (precedes(cur0, lim0) and precedes(cur1, lim1)) reverse_swap_step(lim0, cur1);
    return {lim0, cur1};
}

}
