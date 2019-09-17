#pragma once

#include <initializer_list>

#include "algorithms/copy.h"
#include "concepts/position.h"

namespace elements {

template <Position P>
struct insert_sink
{
    P pos;

    explicit constexpr
    insert_sink(P const& pos_)
        : pos{pos_}
    {}

    constexpr auto
    operator=(Value_type<P> const& x) -> insert_sink&
    {
        pos = insert(pos, x);
        return *this;
    }
};

template <Position P>
struct value_type_t<insert_sink<P>>
{
    using type = Value_type<P>;
};

template <Position P>
constexpr void
increment(insert_sink<P>&)
{}

template <Dynamic_sequence S, Position P>
constexpr auto
insert_range(S const& seq, P pos) -> P
{
    return copy(first(seq), limit(seq), insert_sink{pos}).pos;
}

template <typename T, Position P>
constexpr auto
insert_range(std::initializer_list<T> seq, P pos) -> P
{
    return copy(std::begin(seq), std::end(seq), insert_sink{pos}).pos;
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
