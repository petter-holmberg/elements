#pragma once

#include "cursor.h"

namespace elements {

template <Cursor S, Limit<S> L, Cursor D, Unary_function F>
requires
    Loadable<S> and
    Storable<D> and
    Same_as<Decay<Value_type<S>>, Domain<F>> and
    Same_as<Decay<Value_type<D>>, Codomain<F>>
constexpr auto
map(S src, L lim, D dst, F fun) -> D
//[[expects axiom: not_overlapped_forward(src, lim, dst, dst + (lim - src))]]
{
    while (precedes(src, lim)) {
        store(dst, invoke(fun, load(src)));
        increment(src);
        increment(dst);
    }
    return dst;
}

template <Segmented_cursor S, Cursor D, Unary_function F>
requires
    Loadable<S> and
    Storable<D> and
    Same_as<Decay<Value_type<S>>, Domain<F>> and
    Same_as<Decay<Value_type<D>>, Codomain<F>>
constexpr auto
map(S src, S lim, D dst, F fun) -> D
{
    auto index_src = index_cursor(src);
    auto index_lim = index_cursor(lim);
    if (!precedes(index_src, index_lim)) {
        return map(segment_cursor(src), segment_cursor(lim), dst, fun);
    } else {
        dst = map(segment_cursor(src), limit(load(index_src)), dst, fun);
        do {
            increment(index_src);
            dst = map(first(load(index_src)), limit(load(index_src)), dst, fun);
        } while (precedes(index_src, index_lim));
        return dst;
    }
}

template <Cursor S0, Limit<S0> L0, Cursor S1, Cursor D, Binary_function F>
requires
    Loadable<S0> and
    Loadable<S1> and
    Storable<D> and
    Same_as<Decay<Value_type<S0>>, Decay<Input_type<F, 0>>> and
    Same_as<Decay<Value_type<S1>>, Decay<Input_type<F, 1>>> and
    Same_as<Decay<Value_type<D>>, Codomain<F>>
constexpr auto
map(S0 src0, L0 lim0, S1 src1, D dst, F fun) -> D
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
