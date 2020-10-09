#pragma once

#include "range.h"
#include "search.h"

namespace elements {

template <Cursor C0, Limit<C0> L0, Cursor C1, Limit<C1> L1, Relation<Value_type<C0>, Value_type<C1>> R>
requires
    Loadable<C0> and
    Loadable<C1>
constexpr auto
equivalent_lexicographical(C0 cur0, L0 lim0, C1 cur1, L1 lim1, R rel) -> bool
//[[expects axiom: loadable_range(cur0, lim0)]]
//[[expects axiom: loadable_range(cur0, lim1)]]
//[[expects axiom: equivalence(rel)]]
{
    auto cur{search_mismatch(mv(cur0), lim0, mv(cur1), lim1, rel)};
    return !precedes(get<0>(cur), lim0) and !precedes(get<1>(cur), lim1);
}

template <Cursor C0, Limit<C0> L0, Cursor C1, Limit<C1> L1>
requires
    Loadable<C0> and
    Loadable<C1>
constexpr auto
equal_lexicographical(C0 cur0, L0 lim0, C1 cur1, L1 lim1) -> bool
{
    return equivalent_lexicographical(mv(cur0), lim0, mv(cur1), lim1, equal<Value_type<C0>>{});
}

template <Cursor C0, Limit<C0> L0, Cursor C1, Limit<C1> L1, Relation<Value_type<C0>, Value_type<C1>> R>
requires
    Loadable<C0> and
    Loadable<C1>
constexpr auto
compare_lexicographical(C0 cur0, L0 lim0, C1 cur1, L1 lim1, R rel) -> bool
//[[expects axiom: loadable_range(cur0, lim0)]]
//[[expects axiom: loadable_range(cur0, lim1)]]
//[[expects axiom: weak_ordering(rel)]]
{
    while (true) {
        if (!precedes(cur1, lim1)) return false;
        if (!precedes(cur0, lim0)) return true;
        if (invoke(rel, load(cur0), load(cur1))) return true;
        if (invoke(rel, load(cur1), load(cur0))) return false;
        increment(cur0);
        increment(cur1);
    }
}

template <Cursor C0, Limit<C0> L0, Cursor C1, Limit<C1> L1>
requires
    Loadable<C0> and
    Loadable<C1>
constexpr auto
less_lexicographical(C0 cur0, L0 lim0, C1 cur1, L1 lim1) -> bool
{
    return compare_lexicographical(mv(cur0), lim0, mv(cur1), lim1, less<Value_type<C0>>{});
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
