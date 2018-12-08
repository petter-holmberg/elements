#pragma once

#include "concepts/position.h"

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

}
