#pragma once

#include "search.h"

namespace elements {

template <Cursor C, Limit<C> L, Unary_predicate P>
requires
    Loadable<C> and
    Same_as<Value_type<C>, Domain<P>>
constexpr auto
each_of(C cur, L lim, P pred) -> bool
//[[expects axiom: loadable_range(cur, lim)]]
{
    return !precedes(search_if_not(mv(cur), lim, pred), lim);
}

template <Cursor C, Limit<C> L, Unary_predicate P>
requires
    Loadable<C> and
    Same_as<Value_type<C>, Domain<P>>
constexpr auto
any_not_of(C cur, L lim, P pred) -> bool
//[[expects axiom: loadable_range(cur, lim)]]
{
    return precedes(search_if_not(mv(cur), lim, pred), lim);
}

template <Cursor C, Limit<C> L, Unary_predicate P>
requires
    Loadable<C> and
    Same_as<Value_type<C>, Domain<P>>
constexpr auto
none_of(C cur, L lim, P pred) -> bool
//[[expects axiom: loadable_range(cur, lim)]]
{
    return !precedes(search_if(mv(cur), lim, pred), lim);
}

template <Cursor C, Limit<C> L, Unary_predicate P>
requires
    Loadable<C> and
    Same_as<Value_type<C>, Domain<P>>
constexpr auto
any_of(C cur, L lim, P pred) -> bool
//[[expects axiom: loadable_range(cur, lim)]]
{
    return precedes(search_if(mv(cur), lim, pred), lim);
}

}
