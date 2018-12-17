#pragma once

#include "concepts/position.h"
#include "data_structures/tuple.h"

#include "invocable.h"
#include "position.h"
#include "regular.h"

namespace elements {

template <typename P, typename L, typename U>
requires
    Loadable<P> and
    Limit<P, L> and
    Unary_predicate<U> and
    Same<Remove_cv<Value_type<P>>, Domain<U>>
constexpr auto
search_if(P pos, L lim, U pred) -> P
//[[expects axiom: loadable_range(pos, lim)]]
{
    while (pos != lim) {
        if (pred(load(pos))) break;
        increment(pos);
    }
    return pos;
}

template <typename P, typename L, typename U>
requires
    Loadable<P> and
    Limit<P, L> and
    Unary_predicate<U> and
    Same<Remove_cv<Value_type<P>>, Domain<U>>
constexpr auto
search_if_not(P pos, L lim, U pred) -> P
//[[expects axiom: loadable_range(pos, lim)]]
{
    return search_if(pos, lim, negation<U>{pred});
}

template <typename P, typename L, typename T>
requires
    Loadable<P> and
    Limit<P, L> and
    Equality_comparable_with<Value_type<P>, T>
constexpr auto
search(P pos, L lim, T const& value) -> P
//[[expects axiom: loadable_range(pos, lim)]]
{
    return search_if(pos, lim, equal_unary<Value_type<P>>{value});
}

template <typename P, typename L, typename T>
requires
    Loadable<P> and
    Limit<P, L> and
    Equality_comparable_with<Value_type<P>, T>
constexpr auto
search_not(P pos, L lim, T const& value) -> P
//[[expects axiom: loadable_range(pos, lim)]]
{
    return search_if_not(pos, lim, equal_unary<Value_type<P>>{value});
}

template <typename P, typename U>
requires
    Loadable<P> and
    Position<P> and
    Unary_predicate<U> and
    Same<Remove_cv<Value_type<P>>, Domain<U>>
constexpr auto
search_if_unguarded(P pos, U pred) -> P
//[[expects axiom: loadable_range(pos, _)]]
//[[expects axiom: any_of(pos, _, pred)]]
{
    do {
        if (pred(load(pos))) break;
        increment(pos);
    } while (true);
    return pos;
}

template <typename P, typename U>
requires
    Loadable<P> and
    Position<P> and
    Unary_predicate<U> and
    Same<Remove_cv<Value_type<P>>, Domain<U>>
constexpr auto
search_if_not_unguarded(P pos, U pred) -> P
//[[expects axiom: loadable_range(pos, _)]]
//[[expects axiom: any_not_of(pos, _, pred)]]
{
    return search_if_unguarded(pos, negation<U>{pred});
}

template <typename P, typename T>
requires
    Loadable<P> and
    Position<P> and
    Equality_comparable_with<Value_type<P>, T>
constexpr auto
search_unguarded(P pos, T const& value) -> P
//[[expects axiom: loadable_range(pos, _)]]
//[[expects axiom: search(pos, _, value) != _]]
{
    return search_if_unguarded(pos, equal_unary<Value_type<P>>{value});
}

template <typename P, typename T>
requires
    Loadable<P> and
    Position<P> and
    Equality_comparable_with<Value_type<P>, T>
constexpr auto
search_not_unguarded(P pos, T const& value) -> P
//[[expects axiom: loadable_range(pos, _)]]
//[[expects axiom: search_not(pos, _, value) != _]]
{
    return search_if_not_unguarded(pos, equal_unary<Value_type<P>>{value});
}

template <typename P0, typename L0, typename P1, typename L1, typename Rel = equal<Value_type<P0>>>
requires
    Loadable<P0> and Position<P0> and
    Limit<P0, L0> and
    Loadable<P1> and Position<P1> and
    Limit<P0, L1> and
    Relation<Rel> and
    Same<Remove_cv<Value_type<P0>>, Remove_cv<Value_type<P1>>> and
    Same<Remove_cv<Value_type<P0>>, Domain<Rel>>
constexpr auto
search_match(P0 pos0, L0 lim0, P1 pos1, L1 lim1, Rel rel = equal<Value_type<P0>>{}) -> tuple<P0, P1>
//[[expects axiom: loadable_range(pos0, lim0)]]
//[[expects axiom: loadable_range(pos0, lim1)]]
{
    while (pos0 != lim0 and pos1 != lim1) {
        if (rel(load(pos0), load(pos1))) break;
        increment(pos0);
        increment(pos1);
    }
    return {pos0, pos1};
}

template <typename P0, typename L0, typename P1, typename L1, typename Rel = equal<Value_type<P0>>>
requires
    Loadable<P0> and Position<P0> and
    Limit<P0, L0> and
    Loadable<P1> and Position<P1> and
    Limit<P0, L1> and
    Relation<Rel> and
    Same<Remove_cv<Value_type<P0>>, Remove_cv<Value_type<P1>>> and
    Same<Remove_cv<Value_type<P0>>, Domain<Rel>>
constexpr auto
search_mismatch(P0 pos0, L0 lim0, P1 pos1, L1 lim1, Rel rel = equal<Value_type<P0>>{}) -> tuple<P0, P1>
//[[expects axiom: loadable_range(pos0, lim0)]]
//[[expects axiom: loadable_range(pos0, lim1)]]
{
    return search_match(pos0, lim0, pos1, lim1, complement<Rel>{rel});
}

}
