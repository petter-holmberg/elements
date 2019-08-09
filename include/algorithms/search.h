#pragma once

#include "invocable.h"
#include "position.h"
#include "regular.h"

#include "data_structures/pair.h"

namespace elements {

template <Position P, Limit<P> L, Unary_predicate U>
requires
    Loadable<P> and
    Same<Value_type<P>, Domain<U>>
constexpr auto
search_if(P pos, L lim, U pred) -> P
//[[expects axiom: loadable_range(pos, lim)]]
{
    while (precedes(pos, lim)) {
        if (pred(load(pos))) break;
        increment(pos);
    }
    return pos;
}

template <Position P, Limit<P> L, Unary_predicate U>
requires
    Loadable<P> and
    Same<Decay<Value_type<P>>, Domain<U>>
constexpr auto
search_if_not(P pos, L lim, U pred) -> P
//[[expects axiom: loadable_range(pos, lim)]]
{
    return search_if(mv(pos), mv(lim), negation<U>{pred});
}

template <Position P, Limit<P> L, Equality_comparable_with<Value_type<P>> T>
requires Loadable<P>
constexpr auto
search(P pos, L lim, T const& value) -> P
//[[expects axiom: loadable_range(pos, lim)]]
{
    return search_if(mv(pos), mv(lim), equal_unary<Value_type<P>>{value});
}

template <Position P, Limit<P> L, Equality_comparable_with<Value_type<P>> T>
requires Loadable<P>
constexpr auto
search_not(P pos, L lim, T const& value) -> P
//[[expects axiom: loadable_range(pos, lim)]]
{
    return search_if_not(mv(pos), mv(lim), equal_unary<Value_type<P>>{value});
}

template <Position P, Unary_predicate U>
requires
    Loadable<P> and
    Same<Decay<Value_type<P>>, Domain<U>>
constexpr auto
search_if_unguarded(P pos, U pred) -> P
//[[expects axiom: loadable_range(pos, _)]]
//[[expects axiom: any_of(pos, _, pred)]]
{
    while (!pred(load(pos))) increment(pos);
    return pos;
}

template <Position P, Unary_predicate U>
requires
    Loadable<P> and
    Same<Decay<Value_type<P>>, Domain<U>>
constexpr auto
search_if_not_unguarded(P pos, U pred) -> P
//[[expects axiom: loadable_range(pos, _)]]
//[[expects axiom: any_not_of(pos, _, pred)]]
{
    return search_if_unguarded(mv(pos), negation<U>{pred});
}

template <Position P, Equality_comparable_with<Value_type<P>> T>
requires Loadable<P>
constexpr auto
search_unguarded(P pos, T const& value) -> P
//[[expects axiom: loadable_range(pos, _)]]
//[[expects axiom: search(pos, _, value) != _]]
{
    return search_if_unguarded(mv(pos), equal_unary<Value_type<P>>{value});
}

template <Position P, Equality_comparable_with<Value_type<P>> T>
requires Loadable<P>
constexpr auto
search_not_unguarded(P pos, T const& value) -> P
//[[expects axiom: loadable_range(pos, _)]]
//[[expects axiom: search_not(pos, _, value) != _]]
{
    return search_if_not_unguarded(mv(pos), equal_unary<Value_type<P>>{value});
}

template <Position P0, Limit<P0> L0, Position P1, Limit<P1> L1, Relation Rel = equal<Value_type<P0>>>
requires
    Loadable<P0> and
    Loadable<P1> and
    Same<Decay<Value_type<P0>>, Decay<Value_type<P1>>> and
    Same<Decay<Value_type<P0>>, Domain<Rel>>
constexpr auto
search_match(P0 pos0, L0 lim0, P1 pos1, L1 lim1, Rel rel = equal<Value_type<P0>>{}) -> pair<P0, P1>
//[[expects axiom: loadable_range(pos0, lim0)]]
//[[expects axiom: loadable_range(pos0, lim1)]]
{
    while (precedes(pos0, lim0) and precedes(pos1, lim1)) {
        if (rel(load(pos0), load(pos1))) break;
        increment(pos0);
        increment(pos1);
    }
    return {mv(pos0), mv(pos1)};
}

template <Position P0, Limit<P0> L0, Position P1, Limit<P1> L1, Relation Rel = equal<Value_type<P0>>>
requires
    Loadable<P0> and
    Loadable<P1> and
    Same<Decay<Value_type<P0>>, Decay<Value_type<P1>>> and
    Same<Decay<Value_type<P0>>, Domain<Rel>>
constexpr auto
search_mismatch(P0 pos0, L0 lim0, P1 pos1, L1 lim1, Rel rel = equal<Value_type<P0>>{}) -> pair<P0, P1>
//[[expects axiom: loadable_range(pos0, lim0)]]
//[[expects axiom: loadable_range(pos0, lim1)]]
{
    return search_match(mv(pos0), mv(lim0), mv(pos1), mv(lim1), complement<Rel>{rel});
}

}
