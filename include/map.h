#pragma once

#include "position.h"

namespace elements {

template <Position S, Limit<S> L, Position D, Unary_function Fun>
requires
    Loadable<S> and
    Storable<D> and
    Same_as<Decay<Value_type<S>>, Domain<Fun>> and
    Same_as<Decay<Value_type<D>>, Codomain<Fun>>
constexpr auto
map(S src, L lim, D dst, Fun fun) -> D
//[[expects axiom: not_overlapped_forward(src, lim, dst, dst + (lim - src))]]
{
    while (precedes(src, lim)) {
        store(dst, invoke(fun, load(src)));
        increment(src);
        increment(dst);
    }
    return dst;
}

template <Segmented_position S, Position D, Unary_function Fun>
requires
    Loadable<S> and
    Storable<D> and
    Same_as<Decay<Value_type<S>>, Domain<Fun>> and
    Same_as<Decay<Value_type<D>>, Codomain<Fun>>
constexpr auto
map(S src, S lim, D dst, Fun fun) -> D
{
    auto index_src = index_pos(src);
    auto index_lim = index_pos(lim);
    if (!precedes(index_src, index_lim)) {
        return map(segment_pos(src), segment_pos(lim), dst, fun);
    } else {
        dst = map(segment_pos(src), limit(load(index_src)), dst, fun);
        do {
            increment(index_src);
            dst = map(first(load(index_src)), limit(load(index_src)), dst, fun);
        } while (precedes(index_src, index_lim));
        return dst;
    }
}

template <Position S0, Limit<S0> L0, Position S1, Position D, Binary_function Fun>
requires
    Loadable<S0> and
    Loadable<S1> and
    Storable<D> and
    Same_as<Decay<Value_type<S0>>, Decay<Input_type<Fun, 0>>> and
    Same_as<Decay<Value_type<S1>>, Decay<Input_type<Fun, 1>>> and
    Same_as<Decay<Value_type<D>>, Codomain<Fun>>
constexpr auto
map(S0 src0, L0 lim0, S1 src1, D dst, Fun fun) -> D
//[[expects axiom: not_overlapped_forward(src0, lim0, dst, dst + (lim0 - src0))]]
//[[expects axiom: not_overlapped_forward(src1, _, dst, dst + (_ - src1))]]
{
    while (precedes(src0, lim0)) {
        store(dst, invoke(fun, load(src0), load(src1)));
        increment(src0);
        increment(src1);
        increment(dst);
    }
    return dst;
}

}
