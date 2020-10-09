#pragma once

#include "range.h"

namespace elements {

template <Cursor C, Limit<C> L, Regular_invocable<Value_type<C>> F, typename I = back<Return_type<F, Value_type<C>>>>
requires
    Loadable<C> and
    Sequence<Return_type<F, Value_type<C>>>
constexpr auto
flat_map(C src, L lim, F fun) -> Return_type<F, Value_type<C>>
{
    Return_type<F, Value_type<C>> x;
    while (precedes(src, lim)) {
        auto y = invoke(fun, load(src));
        insert_range(y, I{x});
        increment(src);
    }
    return x;
}

template <Cursor C, Limit<C> L, Regular_invocable<Value_type<C>> F, typename I = back<Return_type<F, Value_type<C>>>>
requires
    Loadable<C> and
    Sequence<Return_type<F, Value_type<C>>> and
    requires (Return_type<F, Value_type<C>> x, Size_type<Return_type<F, Value_type<C>>> s) {
        reserve(x, s);
    }
constexpr auto
flat_map(C src, L lim, F fun) -> Return_type<F, Value_type<C>>
{
    Return_type<F, Value_type<C>> x;
    while (precedes(src, lim)) {
        auto y = invoke(fun, load(src));
        reserve(x, size(x) + size(y));
        insert_range(y, I{x});
        increment(src);
    }
    return x;
}

template <Segmented_cursor C, Regular_invocable<Value_type<C>> F, typename I = back<Return_type<F, Value_type<C>>>>
requires
    Loadable<C> and
    Sequence<Return_type<F, Value_type<C>>>
constexpr auto
flat_map(C src, C lim, F fun) -> Return_type<F, Value_type<C>>
{
    auto index_src = index_cursor(src);
    auto index_lim = index_cursor(lim);
    if (!precedes(index_src, index_lim)) {
        return flat_map(segment_cursor(src), segment_cursor(lim), fun);
    } else {
        auto x = flat_map(segment_cursor(src), limit(load(index_src)), fun);
        do {
            increment(index_src);
            auto y = flat_map(first(load(index_src)), limit(load(index_src)), fun);
            insert_range(y, I{x});
        } while (precedes(index_src, index_lim));
        return x;
    }
}

}
