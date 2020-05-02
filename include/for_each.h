#pragma once

#include "position.h"

namespace elements {

template <Position P, Limit<P> L, Procedure Proc>
requires
    Loadable<P> and
    (Arity<Proc> == 1) and
    Same_as<Value_type<P>, Domain<Proc>>
constexpr auto
for_each(P pos, L lim, Proc proc) -> Proc
//[[expects axiom: loadable_range(pos, lim)]]
{
    while (precedes(pos, lim)) {
        invoke(proc, load(pos));
        increment(pos);
    }
    return proc;
}

template <Segmented_position P, Procedure Proc>
requires
    Loadable<P> and
    (Arity<Proc> == 1) and
    Same_as<Value_type<P>, Domain<Proc>>
constexpr auto
for_each(P pos, P lim, Proc proc) -> Proc
{
    auto index_pos_ = index_pos(pos);
    auto index_lim_ = index_pos(lim);
    if (!precedes(index_pos_, index_lim_)) {
        return for_each(segment_pos(pos), segment_pos(lim), proc);
    } else {
        proc = for_each(segment_pos(pos), limit(load(index_pos_)), proc);
        do {
            increment(index_pos_);
            proc = for_each(first(load(index_pos_)), limit(load(index_pos_)), proc);
        } while (precedes(index_pos_, index_lim_));
        return proc;
    }
}

}
