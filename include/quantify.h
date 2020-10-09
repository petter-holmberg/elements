#pragma once

#include "search.h"

namespace elements {

template <Cursor C, Limit<C> L, Predicate<Value_type<C>> P>
requires Loadable<C>
constexpr auto
each_of(C cur, L lim, P pred) -> bool
//[[expects axiom: loadable_range(cur, lim)]]
{
    return !precedes(search_if_not(mv(cur), lim, pred), lim);
}

template <Cursor C, Limit<C> L, Predicate<Value_type<C>> P>
requires Loadable<C>
constexpr auto
any_not_of(C cur, L lim, P pred) -> bool
//[[expects axiom: loadable_range(cur, lim)]]
{
    return precedes(search_if_not(mv(cur), lim, pred), lim);
}

template <Cursor C, Limit<C> L, Predicate<Value_type<C>> P>
requires Loadable<C>
constexpr auto
none_of(C cur, L lim, P pred) -> bool
//[[expects axiom: loadable_range(cur, lim)]]
{
    return !precedes(search_if(mv(cur), lim, pred), lim);
}

template <Cursor C, Limit<C> L, Predicate<Value_type<C>> P>
requires Loadable<C>
constexpr auto
any_of(C cur, L lim, P pred) -> bool
//[[expects axiom: loadable_range(cur, lim)]]
{
    return precedes(search_if(mv(cur), lim, pred), lim);
}

}
