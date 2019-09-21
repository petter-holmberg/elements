#pragma once

#include "concepts/position.h"

namespace elements {

template <Position P, Unary_predicate U>
requires
    Storable<P> and
    Same<Decay<Value_type<P>>, Domain<U>>
struct filter_position
{
    P pos;
    std::function<Codomain<U>(Input_type<U, 0>)> pred;

    constexpr
    filter_position(P const& pos_, U pred_)
        : pos{pos_}
        , pred{mv(pred_)}
    {}
};

template <Position P, Unary_predicate U>
requires
    Storable<P> and
    Same<Decay<Value_type<P>>, Domain<U>>
struct value_type_t<filter_position<P, U>>
{
    using type = Value_type<P>;
};

template <Position P, Unary_predicate U>
requires
    Storable<P> and
    Same<Decay<Value_type<P>>, Domain<U>>
constexpr auto
precedes(filter_position<P, U> const& pos0, P const& pos1) -> bool
{
    return pos0.pos != pos1;
}

template <Position P, Unary_predicate U>
requires
    Storable<P> and
    Same<Decay<Value_type<P>>, Domain<U>>
constexpr auto
precedes(P const& pos0, filter_position<P, U> const& pos1) -> bool
{
    return !(precedes(pos1, pos0));
}

template <Position P, Unary_predicate U>
requires
    Storable<P> and
    Same<Decay<Value_type<P>>, Domain<U>>
constexpr void
increment(filter_position<P, U>&)
{}

template <Position P, Unary_predicate U>
requires
    Storable<P> and
    Same<Decay<Value_type<P>>, Domain<U>>
constexpr auto
successor(filter_position<P, U> const& pos) -> filter_position<P, U>
{
    return filter_position{successor(pos.pos)};
}

template <Position P, Unary_predicate U>
requires
    Storable<P> and
    Same<Decay<Value_type<P>>, Domain<U>>
constexpr void
store(filter_position<P, U>& pos, Value_type<P> const& x)
{
    if (pos.pred(x)) {
        store(at(pos.pos), x);
        increment(pos.pos);
    }
}

template <Position P, Unary_predicate U>
requires
    Storable<P> and
    Same<Decay<Value_type<P>>, Domain<U>>
constexpr void
store(filter_position<P, U>& pos, Value_type<P>&& x)
{
    if (pos.pred(x)) {
        store(at(pos.pos), fw<Value_type<P>>(x));
        increment(pos.pos);
    }
}

template <Unary_predicate U>
struct filter_sink
{
    std::function<Codomain<U>(Input_type<U, 0>)> pred;

    explicit constexpr
    filter_sink(U pred_)
        : pred{mv(pred_)}
    {}

    template <Position P>
    requires
        Storable<P> and
        Same<Decay<Value_type<P>>, Domain<U>>
    constexpr auto
    operator()(P const& pos)
    {
        return filter_position{pos, pred};
    }
};

}
