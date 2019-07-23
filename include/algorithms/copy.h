#pragma once

#include "position.h"

namespace elements {

template <typename S, typename D>
requires
    Loadable<S> and Position<S> and
    Storable<D> and Position<D> and
    Same<Decay<Value_type<S>>, Decay<Value_type<D>>>
constexpr void
copy_step(S& src, D& dst)
{
    store(dst, load(src));
    increment(dst);
    increment(src);
}

template <typename S, Limit<S> L, typename D>
requires
    Loadable<S> and Position<S> and
    Storable<D> and Position<D> and
    Same<Decay<Value_type<S>>, Decay<Value_type<D>>>
constexpr auto
copy(S src, L lim, D dst) -> D
//[[expects axiom: not_overlapped_forward(src, lim, dst, dst + (lim - src))]]
{
    while (precedes(src, lim)) copy_step(src, dst);
    return dst;
}

}
