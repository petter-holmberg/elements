#pragma once

#include "position.h"

namespace elements {

template <Position S, Limit<S> L, Position D, Unary_function Fun>
requires
    Loadable<S> and
    Storable<D> and
    Same<Decay<Value_type<S>>, Domain<Fun>> and
    Same<Decay<Value_type<D>>, Codomain<Fun>>
constexpr auto
map(S src, L lim, D dst, Fun fun) -> D
//[[expects axiom: not_overlapped_forward(src, lim, dst, dst + (lim - src))]]
{
    while (precedes(src, lim)) {
        store(dst, fun(load(src)));
        increment(src);
        increment(dst);
    }
    return dst;
}

template <Position S0, Limit<S0> L0, Position S1, Position D, Binary_function Fun>
requires
    Loadable<S0> and
    Loadable<S1> and
    Storable<D> and
    Same<Decay<Value_type<S0>>, Decay<Input_type<Fun, 0>>> and
    Same<Decay<Value_type<S1>>, Decay<Input_type<Fun, 1>>> and
    Same<Decay<Value_type<D>>, Codomain<Fun>>
constexpr auto
map(S0 src0, L0 lim0, S1 src1, D dst, Fun fun) -> D
//[[expects axiom: not_overlapped_forward(src0, lim0, dst, dst + (lim0 - src0))]]
//[[expects axiom: not_overlapped_forward(src1, _, dst, dst + (_ - src1))]]
{
    while (precedes(src0, lim0)) {
        store(dst, fun(load(src0), load(src1)));
        increment(src0);
        increment(src1);
        increment(dst);
    }
    return dst;
}

}
