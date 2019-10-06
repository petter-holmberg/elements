#pragma once

#include "search.h"

namespace elements {

template <Position P, Limit<P> L, Unary_predicate U>
requires
    Loadable<P> and
    Same_as<Value_type<P>, Domain<U>>
constexpr auto
each_of(P pos, L lim, U pred) -> bool
//[[expects axiom: loadable_range(pos, lim)]]
{
    return !precedes(search_if_not(mv(pos), lim, pred), lim);
}

template <Position P, Limit<P> L, Unary_predicate U>
requires
    Loadable<P> and
    Same_as<Value_type<P>, Domain<U>>
constexpr auto
any_not_of(P pos, L lim, U pred) -> bool
//[[expects axiom: loadable_range(pos, lim)]]
{
    return precedes(search_if_not(mv(pos), lim, pred), lim);
}

template <Position P, Limit<P> L, Unary_predicate U>
requires
    Loadable<P> and
    Same_as<Value_type<P>, Domain<U>>
constexpr auto
none_of(P pos, L lim, U pred) -> bool
//[[expects axiom: loadable_range(pos, lim)]]
{
    return !precedes(search_if(mv(pos), lim, pred), lim);
}

template <Position P, Limit<P> L, Unary_predicate U>
requires
    Loadable<P> and
    Same_as<Value_type<P>, Domain<U>>
constexpr auto
any_of(P pos, L lim, U pred) -> bool
//[[expects axiom: loadable_range(pos, lim)]]
{
    return precedes(search_if(mv(pos), lim, pred), lim);
}

}
