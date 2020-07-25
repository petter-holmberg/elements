#pragma once

#include "pair.h"
#include "swap.h"

namespace elements {

template <Cursor C, Limit<C> L, Unary_predicate U>
requires
    Loadable<C> and
    Same_as<Value_type<C>, Domain<U>>
constexpr auto
search_if(C cur, L lim, U pred) -> C
//[[expects axiom: loadable_range(cur, lim)]]
{
    while (precedes(cur, lim)) {
        if (invoke(pred, load(cur))) break;
        increment(cur);
    }
    return cur;
}

template <Segmented_cursor C, Unary_predicate U>
requires
    Loadable<C> and
    Same_as<Value_type<C>, Domain<U>>
constexpr auto
search_if(C cur, C lim, U pred) -> C
{
    auto index_cur_ = index_cur(cur);
    auto index_lim_ = index_cur(lim);
    if (!precedes(index_cur_, index_lim_)) {
        return {index_cur_, search_if(segment_cur(cur), segment_cur(lim), pred)};
    } else {
        auto segment_cur_ = search_if(segment_cur(cur), limit(load(index_cur_)), pred);
        if (!precedes(segment_cur_, limit(load(index_cur_)))) return {index_cur_, segment_cur_};
        do {
            increment(index_cur_);
            segment_cur_ = search_if(first(load(index_cur_)), limit(load(index_cur_)), pred);
            if (!precedes(segment_cur_, limit(load(index_cur_)))) return {index_cur_, segment_cur_};
        } while (precedes(index_cur_, index_lim_));
        return {index_cur_, segment_cur_};
    }
}

template <Bidirectional_cursor C, Limit<C> L, Unary_predicate U>
requires
    Loadable<C> and
    Same_as<Decay<Value_type<C>>, Domain<U>>
constexpr auto
search_backward_if(C cur, L lim, U pred) -> C
//[[expects axiom: loadable_range(cur, lim)]]
{
    auto backward_lim = cur;
    advance(cur, lim);
    while (precedes(backward_lim, cur)) {
        if (invoke(pred, load(predecessor(cur)))) break;
        decrement(cur);
    }
    return cur;
}

template <Cursor C, Limit<C> L, Unary_predicate U>
requires
    Loadable<C> and
    Same_as<Decay<Value_type<C>>, Domain<U>>
constexpr auto
search_if_not(C cur, L lim, U pred) -> C
//[[expects axiom: loadable_range(cur, lim)]]
{
    return search_if(mv(cur), mv(lim), negation{pred});
}

template <Segmented_cursor C, Unary_predicate U>
requires
    Loadable<C> and
    Same_as<Value_type<C>, Domain<U>>
constexpr auto
search_if_not(C cur, C lim, U pred) -> C
{
    return search_if(mv(cur), mv(lim), negation{pred});
}

template <Bidirectional_cursor C, Limit<C> L, Unary_predicate U>
requires
    Loadable<C> and
    Same_as<Decay<Value_type<C>>, Domain<U>>
constexpr auto
search_backward_if_not(C cur, L lim, U pred) -> C
//[[expects axiom: loadable_range(cur, lim)]]
{
    return search_backward_if(mv(cur), mv(lim), negation{pred});
}

template <Cursor C, Limit<C> L, Equality_comparable_with<Value_type<C>> T>
requires Loadable<C>
constexpr auto
search(C cur, L lim, T const& value) -> C
//[[expects axiom: loadable_range(cur, lim)]]
{
    return search_if(mv(cur), mv(lim), equal_unary{value});
}

template <Segmented_cursor C, Equality_comparable_with<Value_type<C>> T>
requires Loadable<C>
constexpr auto
search(C cur, C lim, T const& value) -> C
{
    return search_if(mv(cur), mv(lim), equal_unary{value});
}

template <Bidirectional_cursor C, Limit<C> L, Equality_comparable_with<Value_type<C>> T>
requires Loadable<C>
constexpr auto
search_backward(C cur, L lim, T const& value) -> C
//[[expects axiom: loadable_range(cur, lim)]]
{
    return search_backward_if(mv(cur), mv(lim), equal_unary{value});
}

template <Cursor C, Limit<C> L, Equality_comparable_with<Value_type<C>> T>
requires Loadable<C>
constexpr auto
search_not(C cur, L lim, T const& value) -> C
//[[expects axiom: loadable_range(cur, lim)]]
{
    return search_if_not(mv(cur), mv(lim), equal_unary{value});
}

template <Segmented_cursor C, Equality_comparable_with<Value_type<C>> T>
requires Loadable<C>
constexpr auto
search_not(C cur, C lim, T const& value) -> C
{
    return search_if_not(mv(cur), mv(lim), equal_unary{value});
}

template <Bidirectional_cursor C, Limit<C> L, Equality_comparable_with<Value_type<C>> T>
requires Loadable<C>
constexpr auto
search_backward_not(C cur, L lim, T const& value) -> C
//[[expects axiom: loadable_range(cur, lim)]]
{
    return search_backward_if_not(mv(cur), mv(lim), equal_unary{value});
}

template <Cursor C, Unary_predicate U>
requires
    Loadable<C> and
    Same_as<Decay<Value_type<C>>, Domain<U>>
constexpr auto
search_if_unguarded(C cur, U pred) -> C
//[[expects axiom: loadable_range(cur, _)]]
//[[expects axiom: any_of(cur, _, pred)]]
{
    while (!invoke(pred, load(cur))) increment(cur);
    return cur;
}

template <Cursor C, Unary_predicate U>
requires
    Loadable<C> and
    Same_as<Decay<Value_type<C>>, Domain<U>>
constexpr auto
search_if_not_unguarded(C cur, U pred) -> C
//[[expects axiom: loadable_range(cur, _)]]
//[[expects axiom: any_not_of(cur, _, pred)]]
{
    return search_if_unguarded(mv(cur), negation{pred});
}

template <Cursor C, Equality_comparable_with<Value_type<C>> T>
requires Loadable<C>
constexpr auto
search_unguarded(C cur, T const& value) -> C
//[[expects axiom: loadable_range(cur, _)]]
//[[expects axiom: search(cur, _, value) != _]]
{
    return search_if_unguarded(mv(cur), equal_unary{value});
}

template <Cursor C, Equality_comparable_with<Value_type<C>> T>
requires Loadable<C>
constexpr auto
search_not_unguarded(C cur, T const& value) -> C
//[[expects axiom: loadable_range(cur, _)]]
//[[expects axiom: search_not(cur, _, value) != _]]
{
    return search_if_not_unguarded(mv(cur), equal_unary{value});
}

template <Forward_cursor C, Unary_predicate U>
requires
    Mutable<C> and
    Same_as<Decay<Value_type<C>>, Domain<U>>
constexpr auto
search_if_guarded(C first, C last, U pred, Value_type<C> const& sentinel) -> C
//[[expects axiom: mutable_range(cur, successor(last))]]
{
    if (!precedes(first, successor(last))) return first;
    auto temp{load(last)};
    store(last, sentinel);
    auto cur = search_if_unguarded(first, pred);
    swap(at(last), temp);
    if (precedes(cur, last)) return cur;
    if (invoke(pred, at(last))) return last;
    return successor(last);
}

template <Forward_cursor C, Unary_predicate U>
requires
    Mutable<C> and
    Same_as<Decay<Value_type<C>>, Domain<U>>
constexpr auto
search_if_not_guarded(C first, C last, U pred, Value_type<C> const& sentinel) -> C
//[[expects axiom: mutable_range(cur, successor(last))]]
{
    return search_if_guarded(first, last, negation{pred}, sentinel);
}

template <Forward_cursor C, Equality_comparable_with<Value_type<C>> T>
requires Mutable<C>
constexpr auto
search_guarded(C first, C last, T const& value) -> C
//[[expects axiom: mutable_range(cur, successor(last))]]
{
    return search_if_guarded(first, last, equal_unary{value}, value);
}

template <Forward_cursor C, Equality_comparable_with<Value_type<C>> T>
requires Mutable<C>
constexpr auto
search_not_guarded(C first, C last, T const& value, T const& sentinel) -> C
//[[expects axiom: mutable_range(cur, successor(last))]]
//[[expects axiom: value != sentinel]]
{
    return search_if_not_guarded(first, last, equal_unary{value}, sentinel);
}

template <Cursor C0, Limit<C0> L0, Cursor C1, Limit<C1> L1, Relation R = equal<Value_type<C0>>>
requires
    Loadable<C0> and
    Loadable<C1> and
    Same_as<Decay<Value_type<C0>>, Decay<Value_type<C1>>> and
    Same_as<Decay<Value_type<C0>>, Domain<R>>
constexpr auto
search_match(C0 cur0, L0 lim0, C1 cur1, L1 lim1, R rel = {}) -> pair<C0, C1>
//[[expects axiom: loadable_range(cur0, lim0)]]
//[[expects axiom: loadable_range(cur0, lim1)]]
{
    while (precedes(cur0, lim0) and precedes(cur1, lim1)) {
        if (rel(load(cur0), load(cur1))) break;
        increment(cur0);
        increment(cur1);
    }
    return {mv(cur0), mv(cur1)};
}

template <Cursor C0, Limit<C0> L0, Cursor C1, Limit<C1> L1, Relation R = equal<Value_type<C0>>>
requires
    Loadable<C0> and
    Loadable<C1> and
    Same_as<Decay<Value_type<C0>>, Decay<Value_type<C1>>> and
    Same_as<Decay<Value_type<C0>>, Domain<R>>
constexpr auto
search_mismatch(C0 cur0, L0 lim0, C1 cur1, L1 lim1, R rel = {}) -> pair<C0, C1>
//[[expects axiom: loadable_range(cur0, lim0)]]
//[[expects axiom: loadable_range(cur0, lim1)]]
{
    return search_match(mv(cur0), mv(lim0), mv(cur1), mv(lim1), complement{rel});
}

template <Cursor C, Limit<C> L, Relation R = equal<Value_type<C>>>
requires
    Loadable<C> and
    Same_as<Decay<Value_type<C>>, Domain<R>>
constexpr auto
search_adjacent_match(C cur, L lim, R rel = {}) -> C
//[[expects axiom: loadable_range(cur, lim)]]
{
    if (!precedes(cur, lim)) return cur;
    auto x{load(cur)};
    increment(cur);
    while (precedes(cur, lim)) {
        if (rel(x, load(cur))) break;
        x = load(cur);
        increment(cur);
    }
    return cur;
}

template <Cursor C, Limit<C> L, Relation R = equal<Value_type<C>>>
requires
    Loadable<C> and
    Same_as<Decay<Value_type<C>>, Domain<R>>
constexpr auto
search_adjacent_mismatch(C cur, L lim, R rel = {}) -> C
//[[expects axiom: loadable_range(cur, lim)]]
{
    return search_adjacent_match(mv(cur), mv(lim), complement{rel});
}

template <Forward_cursor C, Limit<C> L, Relation R = equal<Value_type<C>>>
requires
    Loadable<C> and
    Same_as<Decay<Value_type<C>>, Domain<R>>
constexpr auto
search_adjacent_match(C cur, L lim, R rel = {}) -> C
//[[expects axiom: loadable_range(cur, lim)]]
{
    if (!precedes(cur, lim)) return cur;
    C prev_cur{cur};
    increment(cur);
    while (precedes(cur, lim)) {
        if (rel(load(prev_cur), load(cur))) break;
        prev_cur = cur;
        increment(cur);
    }
    return cur;
}

template <Forward_cursor C, Limit<C> L, Relation R = equal<Value_type<C>>>
requires
    Loadable<C> and
    Same_as<Decay<Value_type<C>>, Domain<R>>
constexpr auto
search_adjacent_mismatch(C cur, L lim, R rel = {}) -> C
//[[expects axiom: loadable_range(cur, lim)]]
{
    return search_adjacent_match(cur, lim, complement{rel});
}

template <Indexed_cursor C, Indexed_cursor B, Relation R = equal<Value_type<C>>>
requires
    Loadable<C> and
    Loadable<B> and
    Same_as<Difference_type<C>, Value_type<B>> and
    Same_as<Decay<Value_type<C>>, Domain<R>>
constexpr auto
kmp_next(Value_type<B> k, C cur0, C cur1, B buf, R rel = {}) -> Value_type<B>
{
    while (!is_zero(k) and !rel(load(cur0), load(cur1 + k))) k = load(buf + predecessor(k));
    if (rel(load(cur0), load(cur1 + k))) increment(k);
    return k;
}

template <Indexed_cursor C, Limit<C> L, Indexed_cursor B, Relation R = equal<Value_type<C>>>
requires
    Loadable<C> and
    Mutable<B> and
    Same_as<Difference_type<C>, Value_type<B>> and
    Same_as<Decay<Value_type<C>>, Domain<R>>
constexpr void
compute_kmp_next(C cur, L lim, B buf, R rel = {})
{
    auto k{Zero<Value_type<B>>};
    store(buf, k);

    auto first_cur{cur};
    increment(cur);
    auto first_buf{buf};
    while (precedes(cur, lim)) {
        k = kmp_next(k, cur, first_cur, first_buf, rel);
        increment(buf);
        store(buf, k);
        increment(cur);
    }
}

template <Indexed_cursor P, Limit<P> L, Indexed_cursor B, Relation R = equal<Value_type<P>>>
requires
    Loadable<P> and
    Mutable<B> and
    Same_as<Difference_type<P>, Value_type<B>> and
    Same_as<Decay<Value_type<P>>, Domain<R>>
constexpr auto
search_subsequence(P cur0, L lim0, P cur1, L lim1, B buf, R rel = {}) -> P
{
    if (!precedes(cur1, lim1)) return cur0;

    compute_kmp_next(cur1, lim1, buf, rel);
    auto dist{lim1 - cur1};
    auto k{Zero<Value_type<B>>};
    while (precedes(cur0, lim0)) {
        k = kmp_next(k, cur0, cur1, buf, rel);
        if (k == dist) return cur0 - predecessor(k);
        increment(cur0);
    }
    return cur0;
}

}
