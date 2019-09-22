#pragma once

#include <initializer_list>

#include "adapters/insert_position.h"
#include "algorithms/copy.h"
#include "concepts/position.h"

namespace elements {

template <Dynamic_sequence S, Position P>
constexpr auto
insert_range(S const& seq, P pos) -> P
{
    return copy(first(seq), limit(seq), insert_sink{}(pos)).pos;
}

template <typename T, Position P>
constexpr auto
insert_range(std::initializer_list<T> seq, P pos) -> P
{
    return copy(std::begin(seq), std::end(seq), insert_sink{}(pos)).pos;
}

template <Position P, Limit<P> L, Unary_function Fun>
requires
    Loadable<P> and
    Same<Value_type<P>, Domain<Fun>> and
    Dynamic_sequence<Codomain<Fun>>
constexpr auto
flat_map(P src, L lim, Fun fun) -> Codomain<Fun>
{
    Codomain<Fun> x;
    while (precedes(src, lim)) {
        auto y = fun(load(src));
        reserve(x, size(x) + size(y));
        insert_range(y, back{x});
        increment(src);
    }
    return x;
}

}
