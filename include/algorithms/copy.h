#pragma once

#include "position.h"

namespace elements {

template <Loadable_position S, Storable_position D>
requires Same<Decay<Value_type<S>>, Decay<Value_type<D>>>
constexpr void
copy_step(S& src, D& dst)
{
    store(dst, load(src));
    increment(dst);
    increment(src);
}

template <Loadable_position S, Limit<S> L, Storable_position D>
requires Same<Decay<Value_type<S>>, Decay<Value_type<D>>>
constexpr auto
copy(S src, L lim, D dst) -> D
//[[expects axiom: not_overlapped_forward(src, lim, dst, dst + (lim - src))]]
{
    while (src != lim) copy_step(src, dst);
    return dst;
}

}
