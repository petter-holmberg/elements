#pragma once

#include "range.h"

namespace elements {

template <Position P, Limit<P> L, Unary_function Fun, typename I = back<Codomain<Fun>>>
requires
    Loadable<P> and
    Same_as<Value_type<P>, Domain<Fun>> and
    Sequence<Codomain<Fun>>
constexpr auto
flat_map(P src, L lim, Fun fun) -> Codomain<Fun>
{
    Codomain<Fun> x;
    while (precedes(src, lim)) {
        auto y = invoke(fun, load(src));
        insert_range(y, I{x});
        increment(src);
    }
    return x;
}

template <Position P, Limit<P> L, Unary_function Fun, typename I = back<Codomain<Fun>>>
requires
    Loadable<P> and
    Same_as<Value_type<P>, Domain<Fun>> and
    Sequence<Codomain<Fun>> and
    requires (Codomain<Fun> x, Size_type<Codomain<Fun>> s) {
        reserve(x, s);
    }
constexpr auto
flat_map(P src, L lim, Fun fun) -> Codomain<Fun>
{
    Codomain<Fun> x;
    while (precedes(src, lim)) {
        auto y = invoke(fun, load(src));
        reserve(x, size(x) + size(y));
        insert_range(y, I{x});
        increment(src);
    }
    return x;
}

template <Segmented_position P, Unary_function Fun, typename I = back<Codomain<Fun>>>
requires
    Loadable<P> and
    Same_as<Value_type<P>, Domain<Fun>> and
    Sequence<Codomain<Fun>>
constexpr auto
flat_map(P src, P lim, Fun fun) -> Codomain<Fun>
{
    auto index_src = index_pos(src);
    auto index_lim = index_pos(lim);
    if (!precedes(index_src, index_lim)) {
        return flat_map(segment_pos(src), segment_pos(lim), fun);
    } else {
        auto x = flat_map(segment_pos(src), limit(load(index_src)), fun);
        do {
            increment(index_src);
            auto y = flat_map(first(load(index_src)), limit(load(index_src)), fun);
            insert_range(y, I{x});
        } while (precedes(index_src, index_lim));
        return x;
    }
}

}
