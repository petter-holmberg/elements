#pragma once

#include "invocable.h"
#include "partition.h"
#include "range.h"

namespace elements {

template <Relation R>
struct search_binary_lower_predicate
{
    Domain<R> const& value;
    R rel;

    constexpr
    search_binary_lower_predicate(Domain<R> const& value_, R rel_)
        : value{value_}
        , rel{rel_}
    {}

    constexpr auto
    operator()(Domain<R> const& x)
    {
        return !invoke(rel, x, value);
    }
};

template <Relation R>
struct search_binary_upper_predicate
{
    Domain<R> const& value;
    R rel;

    constexpr
    search_binary_upper_predicate(Domain<R> const& value_, R rel_)
        : value{value_}
        , rel{rel_}
    {}

    constexpr auto
    operator()(Domain<R> const& x)
    {
        return invoke(rel, value, x);
    }
};

template <Forward_cursor C, Limit<C> L, Relation R = less<Value_type<C>>>
requires
    Loadable<C> and
    Same_as<Value_type<C>, Domain<R>>
constexpr auto
search_binary_lower(C cur, L lim, Value_type<C> const& value, R rel = {}) -> C
//[[expects axiom: loadable_range(cur, lim)]]
//[[expects axiom: weak_ordering(rel)]]
{
    return partition_point(mv(cur), mv(lim), search_binary_lower_predicate<R>{value, rel});
}

template <Forward_cursor C, Limit<C> L, Relation R = less<Value_type<C>>>
requires
    Loadable<C> and
    Same_as<Value_type<C>, Domain<R>>
constexpr auto
search_binary_upper(C cur, L lim, Value_type<C> const& value, R rel = {}) -> C
//[[expects axiom: loadable_range(cur, lim)]]
//[[expects axiom: weak_ordering(rel)]]
{
    return partition_point(mv(cur), mv(lim), search_binary_upper_predicate<R>{value, rel});
}

template <Forward_cursor C, Limit<C> L, Relation R = less<Value_type<C>>>
requires
    Loadable<C> and
    Same_as<Value_type<C>, Domain<R>>
constexpr auto
search_binary(C cur, L lim, Value_type<C> const& value, R rel = {}) -> bounded_range<C>
//[[expects axiom: loadable_range(cur, lim)]]
//[[expects axiom: weak_ordering(rel)]]
{
    search_binary_lower_predicate<R> first_pred{value, rel};
    search_binary_upper_predicate<R> limit_pred{value, rel};

    auto dist = lim - cur;
    while (!is_zero(dist)) {
        auto const half_dist = half(dist);
        auto mid = cur + half_dist;
        if (limit_pred(load(mid))) {
            dist = half_dist;
        } else if (!first_pred(load(mid))) {
            dist = dist - successor(half_dist);
            cur = successor(mid);
        } else {
            auto first = search_binary_lower(cur, mid, value, rel);
            increment(mid);
            auto limit = search_binary_upper(mid, cur + dist, value, rel);
            return {first, limit};
        }
    }

    return {cur, cur};
}

}
