#pragma once

#include "range.h"

namespace elements {

template <Cursor C, Limit<C> L, Unary_function F, typename I = back<Codomain<F>>>
requires
    Loadable<C> and
    Same_as<Value_type<C>, Domain<F>> and
    Sequence<Codomain<F>>
constexpr auto
flat_map(C src, L lim, F fun) -> Codomain<F>
{
    Codomain<F> x;
    while (precedes(src, lim)) {
        auto y = invoke(fun, load(src));
        insert_range(y, I{x});
        increment(src);
    }
    return x;
}

template <Cursor C, Limit<C> L, Unary_function F, typename I = back<Codomain<F>>>
requires
    Loadable<C> and
    Same_as<Value_type<C>, Domain<F>> and
    Sequence<Codomain<F>> and
    requires (Codomain<F> x, Size_type<Codomain<F>> s) {
        reserve(x, s);
    }
constexpr auto
flat_map(C src, L lim, F fun) -> Codomain<F>
{
    Codomain<F> x;
    while (precedes(src, lim)) {
        auto y = invoke(fun, load(src));
        reserve(x, size(x) + size(y));
        insert_range(y, I{x});
        increment(src);
    }
    return x;
}

template <Segmented_cursor C, Unary_function F, typename I = back<Codomain<F>>>
requires
    Loadable<C> and
    Same_as<Value_type<C>, Domain<F>> and
    Sequence<Codomain<F>>
constexpr auto
flat_map(C src, C lim, F fun) -> Codomain<F>
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
