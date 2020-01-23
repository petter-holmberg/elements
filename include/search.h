#pragma once

#include "pair.h"
#include "swap.h"

namespace elements {

template <Position P, Limit<P> L, Unary_predicate U>
requires
    Loadable<P> and
    Same_as<Value_type<P>, Domain<U>>
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

template <Segmented_position P, Unary_predicate U>
requires
    Loadable<P> and
    Same_as<Value_type<P>, Domain<U>>
constexpr auto
search_if(P pos, P lim, U pred) -> P
{
    auto index_pos = index_pos(pos);
    auto index_lim = index_pos(lim);
    if (!precedes(index_pos, index_lim)) {
        return {index_pos, search_if(segment_pos(pos), segment_pos(lim), pred)};
    } else {
        auto segment_pos = search_if(segment_pos(pos), limit(load(index_pos)), pred);
        if (!precedes(segment_pos, limit(load(index_pos)))) return {index_pos, segment_pos};
        do {
            increment(index_pos);
            segment_pos = search_if(first(load(index_pos)), limit(load(index_pos)), pred);
            if (!precedes(segment_pos, limit(load(index_pos)))) return {index_pos, segment_pos};
        } while (precedes(index_pos, index_lim));
        return {index_pos, segment_pos};
    }
}

template <Position P, Limit<P> L, Unary_predicate U>
requires
    Loadable<P> and
    Same_as<Decay<Value_type<P>>, Domain<U>>
constexpr auto
search_if_not(P pos, L lim, U pred) -> P
//[[expects axiom: loadable_range(pos, lim)]]
{
    return search_if(mv(pos), mv(lim), negation{pred});
}

template <Segmented_position P, Unary_predicate U>
requires
    Loadable<P> and
    Same_as<Value_type<P>, Domain<U>>
constexpr auto
search_if_not(P pos, P lim, U pred) -> P
{
    return search_if(mv(pos), mv(lim), negation{pred});
}

template <Position P, Limit<P> L, Equality_comparable_with<Value_type<P>> T>
requires Loadable<P>
constexpr auto
search(P pos, L lim, T const& value) -> P
//[[expects axiom: loadable_range(pos, lim)]]
{
    return search_if(mv(pos), mv(lim), equal_unary{value});
}

template <Segmented_position P, Equality_comparable_with<Value_type<P>> T>
requires Loadable<P>
constexpr auto
search(P pos, P lim, T const& value) -> P
{
    return search_if(mv(pos), mv(lim), equal_unary{value});
}

template <Position P, Limit<P> L, Equality_comparable_with<Value_type<P>> T>
requires Loadable<P>
constexpr auto
search_not(P pos, L lim, T const& value) -> P
//[[expects axiom: loadable_range(pos, lim)]]
{
    return search_if_not(mv(pos), mv(lim), equal_unary{value});
}

template <Segmented_position P, Equality_comparable_with<Value_type<P>> T>
requires Loadable<P>
constexpr auto
search_not(P pos, P lim, T const& value) -> P
{
    return search_if_not(mv(pos), mv(lim), equal_unary{value});
}

template <Position P, Unary_predicate U>
requires
    Loadable<P> and
    Same_as<Decay<Value_type<P>>, Domain<U>>
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
    Same_as<Decay<Value_type<P>>, Domain<U>>
constexpr auto
search_if_not_unguarded(P pos, U pred) -> P
//[[expects axiom: loadable_range(pos, _)]]
//[[expects axiom: any_not_of(pos, _, pred)]]
{
    return search_if_unguarded(mv(pos), negation{pred});
}

template <Position P, Equality_comparable_with<Value_type<P>> T>
requires Loadable<P>
constexpr auto
search_unguarded(P pos, T const& value) -> P
//[[expects axiom: loadable_range(pos, _)]]
//[[expects axiom: search(pos, _, value) != _]]
{
    return search_if_unguarded(mv(pos), equal_unary{value});
}

template <Position P, Equality_comparable_with<Value_type<P>> T>
requires Loadable<P>
constexpr auto
search_not_unguarded(P pos, T const& value) -> P
//[[expects axiom: loadable_range(pos, _)]]
//[[expects axiom: search_not(pos, _, value) != _]]
{
    return search_if_not_unguarded(mv(pos), equal_unary{value});
}

template <Forward_position P, Unary_predicate U>
requires
    Mutable<P> and
    Same_as<Decay<Value_type<P>>, Domain<U>>
constexpr auto
search_if_guarded(P first, P last, U pred, Value_type<P> const& sentinel) -> P
//[[expects axiom: mutable_range(pos, successor(last))]]
{
    if (!precedes(first, successor(last))) return first;
    auto temp{load(last)};
    store(last, sentinel);
    auto pos = search_if_unguarded(first, pred);
    swap(at(last), temp);
    if (precedes(pos, last)) return pos;
    if (pred(at(last))) return last;
    return successor(last);
}

template <Forward_position P, Unary_predicate U>
requires
    Mutable<P> and
    Same_as<Decay<Value_type<P>>, Domain<U>>
constexpr auto
search_if_not_guarded(P first, P last, U pred, Value_type<P> const& sentinel) -> P
//[[expects axiom: mutable_range(pos, successor(last))]]
{
    return search_if_guarded(first, last, negation{pred}, sentinel);
}

template <Forward_position P, Equality_comparable_with<Value_type<P>> T>
requires Mutable<P>
constexpr auto
search_guarded(P first, P last, T const& value) -> P
//[[expects axiom: mutable_range(pos, successor(last))]]
{
    return search_if_guarded(first, last, equal_unary{value}, value);
}

template <Forward_position P, Equality_comparable_with<Value_type<P>> T>
requires Mutable<P>
constexpr auto
search_not_guarded(P first, P last, T const& value, T const& sentinel) -> P
//[[expects axiom: mutable_range(pos, successor(last))]]
//[[expects axiom: value != sentinel]]
{
    return search_if_not_guarded(first, last, equal_unary{value}, sentinel);
}

template <Position P0, Limit<P0> L0, Position P1, Limit<P1> L1, Relation Rel = equal<Value_type<P0>>>
requires
    Loadable<P0> and
    Loadable<P1> and
    Same_as<Decay<Value_type<P0>>, Decay<Value_type<P1>>> and
    Same_as<Decay<Value_type<P0>>, Domain<Rel>>
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
    Same_as<Decay<Value_type<P0>>, Decay<Value_type<P1>>> and
    Same_as<Decay<Value_type<P0>>, Domain<Rel>>
constexpr auto
search_mismatch(P0 pos0, L0 lim0, P1 pos1, L1 lim1, Rel rel = equal<Value_type<P0>>{}) -> pair<P0, P1>
//[[expects axiom: loadable_range(pos0, lim0)]]
//[[expects axiom: loadable_range(pos0, lim1)]]
{
    return search_match(mv(pos0), mv(lim0), mv(pos1), mv(lim1), complement{rel});
}

template <Position P, Limit<P> L, Relation Rel = equal<Value_type<P>>>
requires
    Loadable<P> and
    Same_as<Decay<Value_type<P>>, Domain<Rel>>
constexpr auto
search_adjacent_match(P pos, L lim, Rel rel = equal<Value_type<P>>{}) -> P
//[[expects axiom: loadable_range(pos, lim)]]
{
    if (!precedes(pos, lim)) return pos;
    auto x{load(pos)};
    increment(pos);
    while (precedes(pos, lim)) {
        if (rel(x, load(pos))) break;
        x = load(pos);
        increment(pos);
    }
    return pos;
}

template <Position P, Limit<P> L, Relation Rel = equal<Value_type<P>>>
requires
    Loadable<P> and
    Same_as<Decay<Value_type<P>>, Domain<Rel>>
constexpr auto
search_adjacent_mismatch(P pos, L lim, Rel rel = equal<Value_type<P>>{}) -> P
//[[expects axiom: loadable_range(pos, lim)]]
{
    return search_adjacent_match(mv(pos), mv(lim), complement{rel});
}

template <Forward_position P, Limit<P> L, Relation Rel = equal<Value_type<P>>>
requires
    Loadable<P> and
    Same_as<Decay<Value_type<P>>, Domain<Rel>>
constexpr auto
search_adjacent_match(P pos, L lim, Rel rel = equal<Value_type<P>>{}) -> P
//[[expects axiom: loadable_range(pos, lim)]]
{
    if (!precedes(pos, lim)) return pos;
    P prev_pos{pos};
    increment(pos);
    while (precedes(pos, lim)) {
        if (rel(load(prev_pos), load(pos))) break;
        prev_pos = pos;
        increment(pos);
    }
    return pos;
}

template <Forward_position P, Limit<P> L, Relation Rel = equal<Value_type<P>>>
requires
    Loadable<P> and
    Same_as<Decay<Value_type<P>>, Domain<Rel>>
constexpr auto
search_adjacent_mismatch(P pos, L lim, Rel rel = equal<Value_type<P>>{}) -> P
//[[expects axiom: loadable_range(pos, lim)]]
{
    return search_adjacent_match(pos, lim, complement{rel});
}

template <Indexed_position P, Indexed_position B, Relation Rel = equal<Value_type<P>>>
requires
    Loadable<P> and
    Loadable<B> and
    Same_as<Difference_type<P>, Value_type<B>> and
    Same_as<Decay<Value_type<P>>, Domain<Rel>>
constexpr auto
kmp_next(Value_type<B> k, P pos0, P pos1, B buf, Rel rel = equal<Value_type<P>>{}) -> Value_type<B>
{
    while (!is_zero(k) and !rel(load(pos0), load(pos1 + k))) k = load(buf + predecessor(k));
    if (rel(load(pos0), load(pos1 + k))) increment(k);
    return k;
}

template <Indexed_position P, Limit<P> L, Indexed_position B, Relation Rel = equal<Value_type<P>>>
requires
    Loadable<P> and
    Mutable<B> and
    Same_as<Difference_type<P>, Value_type<B>> and
    Same_as<Decay<Value_type<P>>, Domain<Rel>>
constexpr void
compute_kmp_next(P pos, L lim, B buf, Rel rel = equal<Value_type<P>>{})
{
    auto k{Zero<Value_type<B>>};
    store(buf, k);

    auto first_pos{pos};
    increment(pos);
    auto first_buf{buf};
    while (precedes(pos, lim)) {
        k = kmp_next(k, pos, first_pos, first_buf, rel);
        increment(buf);
        store(buf, k);
        increment(pos);
    }
}

template <Indexed_position P, Limit<P> L, Indexed_position B, Relation Rel = equal<Value_type<P>>>
requires
    Loadable<P> and
    Mutable<B> and
    Same_as<Difference_type<P>, Value_type<B>> and
    Same_as<Decay<Value_type<P>>, Domain<Rel>>
constexpr auto
search_subsequence(P pos0, L lim0, P pos1, L lim1, B buf, Rel rel = equal<Value_type<P>>{}) -> P
{
    if (!precedes(pos1, lim1)) return pos0;

    compute_kmp_next(pos1, lim1, buf, rel);
    auto dist{lim1 - pos1};
    auto k{Zero<Value_type<B>>};
    while (precedes(pos0, lim0)) {
        k = kmp_next(k, pos0, pos1, buf, rel);
        if (k == dist) return pos0 - predecessor(k);
        increment(pos0);
    }
    return pos0;
}

}
