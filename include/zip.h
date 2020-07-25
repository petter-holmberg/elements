#pragma once

#include "copy.h"
#include "pair.h"

namespace elements {

template <Cursor S0, Limit<S0> L0, Cursor S1, Limit<S1> L1, Cursor D>
requires
    Loadable<S0> and
    Loadable<S1> and
    Storable<D> and
    Same_as<Decay<Value_type<S0>>, Decay<Value_type<D>>> and
    Same_as<Decay<Value_type<S1>>, Decay<Value_type<D>>>
constexpr auto
zip(S0 src0, L0 lim0, S1 src1, L1 lim1, D dst) -> D
//[[expects axiom: not_overlapped_forward(src0, lim0, dst, dst + (lim0 - src0))]]
//[[expects axiom: not_overlapped_forward(src1, lim1, dst, dst + (lim1 - src1))]]
{
    while (precedes(src0, lim0) and precedes(src1, lim1)) {
        copy_step(src0, dst);
        copy_step(src1, dst);
    }
    dst = copy(src0, lim0, dst);
    return copy(src1, lim1, dst);
}

template <Cursor S, Limit<S> L, Cursor D0, Cursor D1>
requires
    Loadable<S> and
    Storable<D0> and
    Storable<D1> and
    Same_as<Decay<Value_type<S>>, Decay<Value_type<D0>>> and
    Same_as<Decay<Value_type<S>>, Decay<Value_type<D1>>>
constexpr auto
unzip(S src, L lim, D0 dst0, D1 dst1) -> pair<D0, D1>
//[[expects axiom: not_overlapped_forward(src, lim, dst0, dst0 + (lim - src))]]
//[[expects axiom: not_overlapped_forward(src, lim, dst1, dst1 + (lim - src))]]
{
    while (true) {
        if (!precedes(src, lim)) break;
        copy_step(src, dst0);
        if (!precedes(src, lim)) break;
        copy_step(src, dst1);
    }
    return {dst0, dst1};
}

}
