#pragma once

#include "invocable.h"
#include "position.h"
#include "regular.h"
#include "partition.h"
#include "data_structures/bounded_range.h"

namespace elements {

template <Relation Rel>
struct search_binary_lower_predicate
{
    Domain<Rel> const& value;
    Rel rel;

    constexpr
    search_binary_lower_predicate(Domain<Rel> const& value_, Rel rel_)
        : value{value_}
        , rel{rel_}
    {}

    constexpr auto
    operator()(Domain<Rel> const& x)
    {
        return !rel(x, value);
    }
};

template <Relation Rel>
struct search_binary_upper_predicate
{
    Domain<Rel> const& value;
    Rel rel;

    constexpr
    search_binary_upper_predicate(Domain<Rel> const& value_, Rel rel_)
        : value{value_}
        , rel{rel_}
    {}

    constexpr auto
    operator()(Domain<Rel> const& x)
    {
        return rel(value, x);
    }
};

template <Forward_position P, Limit<P> L, Relation Rel = less<Value_type<P>>>
requires
    Loadable<P> and
    Same<Value_type<P>, Domain<Rel>>
constexpr auto
search_binary_lower(P pos, L lim, Value_type<P> const& value, Rel rel = less<Value_type<P>>{}) -> P
//[[expects axiom: loadable_range(pos, lim)]]
//[[expects axiom: weak_ordering(rel)]]
{
    return partition_point(mv(pos), mv(lim), search_binary_lower_predicate<Rel>{value, rel});
}

template <Forward_position P, Limit<P> L, Relation Rel = less<Value_type<P>>>
requires
    Loadable<P> and
    Same<Value_type<P>, Domain<Rel>>
constexpr auto
search_binary_upper(P pos, L lim, Value_type<P> const& value, Rel rel = less<Value_type<P>>{}) -> P
//[[expects axiom: loadable_range(pos, lim)]]
//[[expects axiom: weak_ordering(rel)]]
{
    return partition_point(mv(pos), mv(lim), search_binary_upper_predicate<Rel>{value, rel});
}

template <Forward_position P, Limit<P> L, Relation Rel = less<Value_type<P>>>
requires
    Loadable<P> and
    Same<Value_type<P>, Domain<Rel>>
constexpr auto
search_binary(P pos, L lim, Value_type<P> const& value, Rel rel = less<Value_type<P>>{}) -> bounded_range<P>
//[[expects axiom: loadable_range(pos, lim)]]
//[[expects axiom: weak_ordering(rel)]]
{
    search_binary_lower_predicate<Rel> first_pred{value, rel};
    search_binary_upper_predicate<Rel> limit_pred{value, rel};

    auto dist = lim - pos;
    while (!is_zero(dist)) {
        auto const half_dist = half(dist);
        auto mid = pos + half_dist;
        if (limit_pred(load(mid))) {
            dist = half_dist;
        } else if (!first_pred(load(mid))) {
            dist = dist - successor(half_dist);
            pos = successor(mid);
        } else {
            auto first = search_binary_lower(pos, mid, value, rel);
            increment(mid);
            auto limit = search_binary_upper(mid, pos + dist, value, rel);
            return {first, limit};
        }
    }

    return {pos, pos};
}

}
