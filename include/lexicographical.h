#pragma once

#include "range.h"
#include "search.h"

namespace elements {

template <Position P0, Limit<P0> L0, Position P1, Limit<P1> L1, Relation Rel>
requires
    Loadable<P0> and
    Loadable<P1> and
    Same_as<Decay<Value_type<P0>>, Decay<Value_type<P1>>> and
    Same_as<Decay<Value_type<P0>>, Domain<Rel>>
constexpr auto
equivalent_lexicographical(P0 pos0, L0 lim0, P1 pos1, L1 lim1, Rel rel) -> bool
//[[expects axiom: loadable_range(pos0, lim0)]]
//[[expects axiom: loadable_range(pos0, lim1)]]
//[[expects axiom: equivalence(rel)]]
{
    auto pos{search_mismatch(mv(pos0), lim0, mv(pos1), lim1, rel)};
    return !precedes(get<0>(pos), lim0) and !precedes(get<1>(pos), lim1);
}

template <Position P0, Limit<P0> L0, Position P1, Limit<P1> L1>
requires
    Loadable<P0> and
    Loadable<P1> and
    Same_as<Decay<Value_type<P0>>, Decay<Value_type<P1>>>
constexpr auto
equal_lexicographical(P0 pos0, L0 lim0, P1 pos1, L1 lim1) -> bool
{
    return equivalent_lexicographical(mv(pos0), lim0, mv(pos1), lim1, equal<Value_type<P0>>{});
}

template <Position P0, Limit<P0> L0, Position P1, Limit<P1> L1, Relation Rel>
requires
    Loadable<P0> and
    Loadable<P1> and
    Same_as<Decay<Value_type<P0>>, Decay<Value_type<P1>>> and
    Same_as<Decay<Value_type<P0>>, Domain<Rel>>
constexpr auto
compare_lexicographical(P0 pos0, L0 lim0, P1 pos1, L1 lim1, Rel rel) -> bool
//[[expects axiom: loadable_range(pos0, lim0)]]
//[[expects axiom: loadable_range(pos0, lim1)]]
//[[expects axiom: weak_ordering(rel)]]
{
    while (true) {
        if (!precedes(pos1, lim1)) return false;
        if (!precedes(pos0, lim0)) return true;
        if (invoke(rel, load(pos0), load(pos1))) return true;
        if (invoke(rel, load(pos1), load(pos0))) return false;
        increment(pos0);
        increment(pos1);
    }
}

template <Position P0, Limit<P0> L0, Position P1, Limit<P1> L1>
requires
    Loadable<P0> and
    Loadable<P1> and
    Same_as<Decay<Value_type<P0>>, Decay<Value_type<P1>>>
constexpr auto
less_lexicographical(P0 pos0, L0 lim0, P1 pos1, L1 lim1) -> bool
{
    return compare_lexicographical(mv(pos0), lim0, mv(pos1), lim1, less<Value_type<P0>>{});
}

template <Range R>
constexpr auto
equal_range(R const& x, R const& y) -> bool
{
    return equal_lexicographical(first(x), limit(x), first(y), limit(y));
}

template <Range R>
constexpr auto
less_range(R const& x, R const& y) -> bool
{
    return less_lexicographical(first(x), limit(x), first(y), limit(y));
}

}
