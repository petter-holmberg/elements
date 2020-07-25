#pragma once

#include "cursor.h"

namespace elements {

template <Cursor C, Procedure P>
struct for_each_result
{
    C cursor;
    P procedure;

    constexpr
    for_each_result()
        : cursor()
        , procedure()
    {}

    constexpr
    for_each_result(C cur, P proc)
        : cursor(mv(cur))
        , procedure(mv(proc))
    {}
};

template <Cursor C, Limit<C> L, Procedure P>
requires
    Loadable<C> and
    (Arity<P> == 1) and
    Convertible_to<Value_type<C>, Domain<P>>
constexpr auto
for_each(C cur, L lim, P proc) -> for_each_result<C, P>
//[[expects axiom: loadable_range(cur, lim)]]
{
    while (precedes(cur, lim)) {
        invoke(proc, load(cur));
        increment(cur);
    }
    return {mv(cur), mv(proc)};
}

template <Segmented_cursor C, Procedure P>
requires
    Loadable<C> and
    (Arity<P> == 1) and
    Convertible_to<Value_type<C>, Domain<P>>
constexpr auto
for_each(C cur, C lim, P proc) -> for_each_result<C, P>
{
    auto index_cur = index_cursor(cur);
    auto index_lim = index_cursor(lim);
    if (!precedes(index_cur, index_lim)) {
        return for_each(segment_cursor(cur), segment_cursor(lim), proc);
    } else {
        proc = for_each(segment_cursor(cur), limit(load(index_cur)), proc);
        do {
            increment(index_cur);
            proc = for_each(first(load(index_cur)), limit(load(index_cur)), proc);
        } while (precedes(index_cur, index_lim));
        return {mv(cur), mv(proc)};
    }
}

}
