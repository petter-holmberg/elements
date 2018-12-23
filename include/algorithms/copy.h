#pragma once

#include "position.h"

namespace elements {

template <typename S, typename D>
requires
    Loadable<S> and Position<S> and
    Storable<D> and Position<D> and
    Same<Remove_cv<Value_type<S>>, Value_type<D>>
constexpr void
copy_step(S& src, D& dst)
{
    store(dst, load(src));
    increment(dst);
    increment(src);
}

template <typename S, typename L, typename D>
requires
    Loadable<S> and
    Limit<S, L> and
    Storable<D> and Position<D> and
    Same<Remove_cv<Value_type<S>>, Value_type<D>>
constexpr auto
copy(S src, L lim, D dst) -> D
//[[expects axiom: not_overlapped_forward(src, lim, dst, dst + (lim - src))]]
{
    while (src != lim) copy_step(src, dst);
    return dst;
}

}
