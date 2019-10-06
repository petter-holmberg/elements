#pragma once

#include "position.h"

namespace elements {

template <Position P, Limit<P> L, Procedure Proc>
requires
    Loadable<P> and
    Arity<Proc> == 1 and
    Same_as<Value_type<P>, Domain<Proc>>
constexpr auto
for_each(P pos, L lim, Proc proc) -> Proc
//[[expects axiom: loadable_range(pos, lim)]]
{
    while (precedes(pos, lim)) {
        proc(load(pos));
        increment(pos);
    }
    return proc;
}

}
