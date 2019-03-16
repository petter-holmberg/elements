#pragma once

#include "regular.h"
#include "position.h"
#include "search.h"

namespace elements {

template <
    Loadable_position P0, Limit<P0> L0,
    Loadable_position P1, Limit<P1> L1,
    Relation Rel>
requires
    Same<Decay<Value_type<P0>>, Decay<Value_type<P1>>> and
    Same<Decay<Value_type<P0>>, Decay<Domain<Rel>>>
constexpr auto
lexicographical_equivalent(P0 pos0, L0 lim0, P1 pos1, L1 lim1, Rel rel) -> bool
//[[expects axiom: loadable_range(pos0, lim0)]]
//[[expects axiom: loadable_range(pos0, lim1)]]
//[[expects axiom: equivalence(rel)]]
{
    auto pos{search_mismatch(pos0, lim0, pos1, lim1, rel)};
    return !precedes(elements::get<0>(pos), lim0) and !precedes(elements::get<1>(pos), lim1);
}

template <
    Loadable_position P0, Limit<P0> L0,
    Loadable_position P1, Limit<P1> L1>
requires Same<Decay<Value_type<P0>>, Decay<Value_type<P1>>>
constexpr auto
lexicographical_equal(P0 pos0, L0 lim0, P1 pos1, L1 lim1) -> bool
{
    return lexicographical_equivalent(pos0, lim0, pos1, lim1, equal<Value_type<P0>>{});
}

template <
    Loadable_position P0, Limit<P0> L0,
    Loadable_position P1, Limit<P1> L1,
    Relation Rel>
requires
    Same<Decay<Value_type<P0>>, Decay<Value_type<P1>>> and
    Same<Decay<Value_type<P0>>, Decay<Domain<Rel>>>
constexpr auto
lexicographical_compare(P0 pos0, L0 lim0, P1 pos1, L1 lim1, Rel rel) -> bool
//[[expects axiom: loadable_range(pos0, lim0)]]
//[[expects axiom: loadable_range(pos0, lim1)]]
//[[expects axiom: weak_ordering(rel)]]
{
    while (true) {
        if (!precedes(pos1, lim1)) return false;
        if (!precedes(pos0, lim0)) return true;
        if (rel(load(pos0), load(pos1))) return true;
        if (rel(load(pos1), load(pos0))) return false;
        increment(pos0);
        increment(pos1);
    }
}

template <
    Loadable_position P0, Limit<P0> L0,
    Loadable_position P1, Limit<P1> L1>
requires Same<Decay<Value_type<P0>>, Decay<Value_type<P1>>>
constexpr auto
lexicographical_less(P0 pos0, L0 lim0, P1 pos1, L1 lim1) -> bool
{
    return lexicographical_compare(pos0, lim0, pos1, lim1, less<Value_type<P0>>{});
}

}
